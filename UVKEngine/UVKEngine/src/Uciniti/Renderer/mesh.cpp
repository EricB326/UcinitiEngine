#include "uvkpch.h"
#include "mesh.h"

#include <filesystem>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "Uciniti/Renderer/renderer.h"
#include "Uciniti/Renderer/renderer_api.h"
#include "Uciniti/Platform/Vulkan/vulkan_mesh.h"

namespace Uciniti
{
	static const uint32_t _mesh_import_flags =
		aiProcess_CalcTangentSpace |	  // Create binormals/tangents if non-existent.
		aiProcess_Triangulate |			  // Make sure the mesh is in triangles.
		aiProcess_SortByPType |			  // Split meshes into primitive type groups.
		aiProcess_GenNormals |			  // Verify the normals are in.
		aiProcess_GenUVCoords |			  // Convert UVs where required.
		//aiProcess_OptimizeMeshes |	  // Batch any draws where possible.
		aiProcess_JoinIdenticalVertices | // Connect any overlapping vertices.
		aiProcess_ValidateDataStructure;  // Validation

	ref_ptr<mesh> mesh::create(const std::string& a_file_path)
	{
		switch (renderer_api::get_current_api())
		{
			case renderer_api_type::none: return nullptr;
			case renderer_api_type::vulkan: return create_ref_ptr<vulkan_mesh>(a_file_path);
		}

		UVK_CORE_ASSERT(false, "Unknown Rendering API type.");
		return nullptr;
	}

	void mesh::load_mesh(const std::string& a_file_path)
	{
		_file_path = a_file_path;
		UVK_CORE_INFO("Loading mesh: {0}", _file_path.c_str());

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(_file_path, _mesh_import_flags);
		if (!scene || !scene->HasMeshes() || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
			UVK_CORE_ERROR("Failed to load mesh file: {0}", _file_path);
		_scene = scene;

		_mesh_shader = renderer::get_shader_library()->get("static_shader");

		uint32_t vertex_count = 0;
		uint32_t index_count = 0;

		_submeshes.reserve(_scene->mNumMeshes);
		for (size_t i = 0; i < _scene->mNumMeshes; i++)
		{
			aiMesh* this_mesh = _scene->mMeshes[i];

			submesh& this_submesh = _submeshes.emplace_back();
			this_submesh._base_vertex = vertex_count;
			this_submesh._base_index = index_count;
			this_submesh._material_index = this_mesh->mMaterialIndex;
			this_submesh._index_count = this_mesh->mNumFaces * 3;
			this_submesh._mesh_name = this_mesh->mName.C_Str();

			vertex_count += this_mesh->mNumVertices;
			index_count += this_submesh._index_count;

			UVK_CORE_ASSERT(this_mesh->HasPositions(), "Meshes require positions.");
			UVK_CORE_ASSERT(this_mesh->HasNormals(), "Meshes require normals.");

			// Vertices.
			for (size_t j = 0; j < this_mesh->mNumVertices; j++)
			{
				vertex this_vertex;
				this_vertex._position = { this_mesh->mVertices[j].x, this_mesh->mVertices[j].y, this_mesh->mVertices[j].z };
				this_vertex._normal = { this_mesh->mNormals[j].x, this_mesh->mNormals[j].y, this_mesh->mNormals[j].z };
			
				if (this_mesh->HasTangentsAndBitangents())
				{
					this_vertex._tangent = { this_mesh->mTangents[j].x, this_mesh->mTangents[j].y, this_mesh->mTangents[j].z };
					this_vertex._binormal = { this_mesh->mBitangents[j].x, this_mesh->mBitangents[j].y, this_mesh->mBitangents[j].z };
				}

				if (this_mesh->HasTextureCoords(0))
					this_vertex._tex_coord = { this_mesh->mTextureCoords[0][j].x, this_mesh->mTextureCoords[0][j].y };

				_static_vertices.push_back(this_vertex);
			}

			// Indices.
			for (size_t j = 0; j < this_mesh->mNumFaces; j++)
			{
				UVK_CORE_ASSERT(this_mesh->mFaces[j].mNumIndices == 3, "Faces must have 3 indices.");
				index this_index = { this_mesh->mFaces[j].mIndices[0], this_mesh->mFaces[j].mIndices[1], this_mesh->mFaces[j].mIndices[2] };
				_indices.push_back(this_index);
			}
		}

		traverse_nodes(_scene->mRootNode);

		read_materials();

		_vertex_buffer = vertex_buffer::create(_static_vertices.data(), _static_vertices.size() * sizeof(vertex), _static_vertices.size());
		_vertex_buffer_layout =
		{
			{ shader_data_type::_float3, "in_vert_position" },
			{ shader_data_type::_float3, "in_vert_normal" },
			{ shader_data_type::_float3, "in_vert_tangent" },
			{ shader_data_type::_float3, "in_vert_binormal" },
			{ shader_data_type::_float2, "in_vert_tex_coord" },
		};

		_index_buffer = index_buffer::create(_indices.data(), _indices.size() * sizeof(index));
	}

	void mesh::traverse_nodes(aiNode* a_node)
	{
		for (size_t i = 0; i < a_node->mNumMeshes; i++)
		{
			uint32_t this_mesh = a_node->mMeshes[i];
			submesh& this_submesh = _submeshes[this_mesh];
			this_submesh._node_name = a_node->mName.C_Str();
		}

		for (size_t i = 0; i < a_node->mNumChildren; i++)
			traverse_nodes(a_node->mChildren[i]);
	}

	void mesh::read_materials()
	{
		if (_scene->HasMaterials())
		{
			UVK_CORE_TRACE("---- Materials - {0} ----", _file_path);

			_textures.resize(_scene->mNumMaterials);

			for (uint32_t i = 0; i < _scene->mNumMaterials; i++)
			{
				aiMaterial* this_mat = _scene->mMaterials[i];
				if (!this_mat)
					break; // #TODO: Shout in the console that the mat failed to be retrieved.
				aiString mat_name = this_mat->GetName();

				UVK_CORE_TRACE("\t{0} (index = {1})", mat_name.data, i);
				aiString mat_tex_path;
				uint32_t tex_count = this_mat->GetTextureCount(aiTextureType_DIFFUSE);
				UVK_CORE_TRACE("\tTexture count = {0}", tex_count);

				aiColor3D colour;
				this_mat->Get(AI_MATKEY_COLOR_DIFFUSE, colour);

				float shininess, metalness, roughness;

				if (this_mat->Get(AI_MATKEY_SHININESS, shininess) != aiReturn_SUCCESS)
					shininess = 80.0f; // Default value.

				if (this_mat->Get(AI_MATKEY_REFLECTIVITY, metalness) != aiReturn_SUCCESS)
					metalness = 0.0f;

				roughness = 1.0f - glm::sqrt(shininess / 100.0f);
				UVK_CORE_TRACE("\tCOLOUR = {0}, {1}, {2}", colour.r, colour.g, colour.b);
				UVK_CORE_TRACE("\tROUGHNESS = {0}", roughness);

				bool has_albedo = this_mat->GetTexture(aiTextureType_DIFFUSE, 0, &mat_tex_path) == AI_SUCCESS;
				if (has_albedo)
				{
					std::filesystem::path path = _file_path;
					std::filesystem::path parent_path = path.parent_path();
					parent_path /= std::string(mat_tex_path.data);
					std::string tex_path = parent_path.string();
					UVK_CORE_TRACE("\tAlbedo map path = {0}", tex_path);

					ref_ptr<texture2D> albedo_tex = texture2D::create(tex_path);
					if (albedo_tex->has_loaded())
						_textures[i] = albedo_tex;
				}
			}
		}
	}

}