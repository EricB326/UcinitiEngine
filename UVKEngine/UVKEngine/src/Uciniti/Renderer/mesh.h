#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>

#include "Uciniti/Renderer/pipeline.h"
#include "Uciniti/Renderer/buffer.h"
#include "Uciniti/Renderer/shader.h"
#include "Uciniti/Renderer/texture.h"

struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
struct aiScene;

namespace Uciniti
{
	struct vertex
	{
		glm::vec3 _position;
		glm::vec3 _normal;
		glm::vec3 _tangent;
		glm::vec3 _binormal;
		glm::vec2 _tex_coord;
	};

	struct index
	{
		uint32_t v1, v2, v3;
	};

	static_assert(sizeof(index) == 3 * sizeof(uint32_t));

	class submesh
	{
	public:
		uint32_t _base_vertex;
		uint32_t _base_index;
		uint32_t _material_index;
		uint32_t _index_count;
		
		glm::mat4 _transform;

		std::string _node_name, _mesh_name;
	};

	class mesh
	{
	public:
		static ref_ptr<mesh> create(const std::string& a_file_path);
		
		virtual std::vector<submesh>& get_submeshes() = 0;

		virtual ref_ptr<shader> get_mesh_shader() = 0;
		virtual const std::string& get_file_path() const = 0;

		virtual ref_ptr<vertex_buffer> get_vertex_buffer() = 0;
		virtual ref_ptr<index_buffer> get_index_buffer() = 0;

	protected:
		std::vector<submesh> _submeshes;
		
		ref_ptr<pipeline> _pipeline;
		ref_ptr<vertex_buffer> _vertex_buffer;
		ref_ptr<index_buffer> _index_buffer;
		buffer_layout _vertex_buffer_layout;
		
		std::vector<vertex> _static_vertices;
		std::vector<index> _indices;
		const aiScene* _scene;

		ref_ptr<shader> _mesh_shader;
		std::vector<ref_ptr<texture2D>> _textures;

		std::string _file_path;

		void load_mesh(const std::string& a_file_path);
		void traverse_nodes(aiNode* a_node);
		void read_materials();
	};
}

#endif // !MESH_H