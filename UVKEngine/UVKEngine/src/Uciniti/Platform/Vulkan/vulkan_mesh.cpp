#include "uvkpch.h"
#include "vulkan_mesh.h"

#include "Uciniti/Platform/Vulkan/vulkan_shader.h"
#include "Uciniti/Platform/Vulkan/vulkan_context.h"

namespace Uciniti
{

	vulkan_mesh::vulkan_mesh(const std::string& a_file_path)
	{
		load_mesh(a_file_path);

		write_mesh_descriptors();
	}

	void vulkan_mesh::write_mesh_descriptors()
	{
		pipeline_spec pipeline_specification;
		pipeline_specification._shader = _mesh_shader;

		ref_ptr<vulkan_shader> vulkan_mesh_shader = std::static_pointer_cast<vulkan_shader>(_mesh_shader);

		_descriptor_set = vulkan_mesh_shader->create_descriptor_set();

		std::vector<VkWriteDescriptorSet> write_descriptors = vulkan_mesh_shader->get_write_descriptor_sets();

		for (VkWriteDescriptorSet& this_write : write_descriptors)
		{
			this_write.dstSet = _descriptor_set;
			
			switch (this_write.descriptorType)
			{
			case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
				this_write.pBufferInfo = &vulkan_mesh_shader->get_buffer_info(this_write.dstBinding);
				_write_descriptor_sets.push_back(this_write);
				break;
			default:
				break;
			}

		}

		const VkWriteDescriptorSet* wds = vulkan_mesh_shader->get_write_descriptor_set("in_frag_albedo_tex");
		if (wds)
		{
			VkWriteDescriptorSet write_descriptor = *wds;
			write_descriptor.dstSet = _descriptor_set;
			write_descriptor.pImageInfo = &std::static_pointer_cast<vulkan_texture2D>(_textures[0])->get_descriptor();
			_write_descriptor_sets.push_back(write_descriptor);
		}

		UVK_CORE_WARN("Updating {0} descriptor sets", _write_descriptor_sets.size());
		vkUpdateDescriptorSets(vulkan_context::get()->get_logical_device()->get_logical_device(), _write_descriptor_sets.size(), _write_descriptor_sets.data(), 0, nullptr);
	}
}