#ifndef VULKAN_MESH_H
#define VULKAN_MESH_H

#include "Uciniti/Renderer/mesh.h"

#include "Uciniti/Platform/Vulkan/vulkan_base.h"

namespace Uciniti
{
	class vulkan_mesh : public mesh
	{
	public:
		vulkan_mesh(const std::string& a_file_path);

		virtual std::vector<submesh>& get_submeshes() override { return _submeshes; }

		virtual ref_ptr<shader> get_mesh_shader() override { return _mesh_shader; }
		virtual const std::string& get_file_path() const override { return _file_path; }

		virtual ref_ptr<vertex_buffer> get_vertex_buffer() override { return _vertex_buffer; }
		virtual ref_ptr<index_buffer> get_index_buffer() override { return _index_buffer; }

		VkDescriptorSet* get_descriptor_set() { return &_descriptor_set; }

		virtual const glm::mat4& get_transform() const override { return _transform; }
		virtual void set_transform(const glm::mat4& a_transform) override { _transform = a_transform; }

	private:
		VkDescriptorSet _descriptor_set;
		std::vector<VkWriteDescriptorSet> _write_descriptor_sets;

		void write_mesh_descriptors();
	};
}

#endif // !VULKAN_MESH_H