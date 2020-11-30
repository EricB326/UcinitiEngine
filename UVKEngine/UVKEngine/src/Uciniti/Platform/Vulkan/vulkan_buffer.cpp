#include "uvkpch.h"
#include "vulkan_buffer.h"
#include "Uciniti/Platform/Vulkan/vulkan_context.h"
#include "vulkan_create_info_base.h"
namespace Uciniti
{
	// =================================================================
	// Vulkan Buffer Utility
	// =================================================================
	void vulkan_buffer_util::create_buffer(vulkan_buffer_type a_buffer_type, VkBuffer& a_buffer, VkDeviceMemory& a_device_mem, VkDeviceSize a_buffer_size)
	{
		ref_ptr<vulkan_logical_device> device = vulkan_context::get()->get_logical_device();

		VkBufferCreateInfo buffer_info(vk_base_buffer_create_info);
		VkMemoryPropertyFlags property_flags;

		switch (a_buffer_type)
		{
			case Uciniti::vulkan_buffer_type::VERTEX:
				buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
				property_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				break;
			case Uciniti::vulkan_buffer_type::INDEX:
				buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
				property_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				break;
			case Uciniti::vulkan_buffer_type::STAGING:
				buffer_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				property_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				break;
			case Uciniti::vulkan_buffer_type::UNIFORM:
				buffer_info.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
				property_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
				break;
			default:
				break;
		}

		buffer_info.size = a_buffer_size;
		buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VK_CHECK_RESULT(vkCreateBuffer(device->get_logical_device(), &buffer_info, nullptr, &a_buffer));

		VkMemoryRequirements mem_requirements;
		vkGetBufferMemoryRequirements(device->get_logical_device(), a_buffer, &mem_requirements);

		// #TODO: Allocate memory shouldn't be called for each buffer. Should setup a custom allocator that splits up for large
		// numbers of objects and split up a single allocation among many different objects using offsets.
		VkMemoryAllocateInfo mem_alloc(vk_base_memory_alloc_info);
		mem_alloc.allocationSize = mem_requirements.size;
		mem_alloc.memoryTypeIndex = device->get_physical_device_ref()->get_memory_type_index(mem_requirements.memoryTypeBits, property_flags);
		VK_CHECK_RESULT(vkAllocateMemory(device->get_logical_device(), &mem_alloc, nullptr, &a_device_mem));

		VK_CHECK_RESULT(vkBindBufferMemory(device->get_logical_device(), a_buffer, a_device_mem, 0));
	}

	void vulkan_buffer_util::copy_buffer(VkBuffer a_src_buffer, VkBuffer a_dst_buffer, VkDeviceSize a_buffer_size)
	{
		ref_ptr<vulkan_logical_device> device = vulkan_context::get()->get_logical_device();

		VkCommandBuffer command_buffer = begin_one_time_command();

		VkBufferCopy copy_region{};
		copy_region.size = a_buffer_size;
		vkCmdCopyBuffer(command_buffer, a_src_buffer, a_dst_buffer, 1, &copy_region);

		end_one_time_command(command_buffer);
	}

	VkCommandBuffer vulkan_buffer_util::begin_one_time_command()
	{
		ref_ptr<vulkan_logical_device> device = vulkan_context::get()->get_logical_device();

		VkCommandBufferAllocateInfo alloc_info(vk_base_command_buffer_alloc_info);
		alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandPool = device->get_command_pool();
		alloc_info.commandBufferCount = 1;

		VkCommandBuffer command_buffer;
		vkAllocateCommandBuffers(device->get_logical_device(), &alloc_info, &command_buffer);

		VkCommandBufferBeginInfo begin_info(vk_base_command_buffer_begin_info);
		begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(command_buffer, &begin_info);

		return command_buffer;
	}

	void vulkan_buffer_util::end_one_time_command(VkCommandBuffer a_command_buffer)
	{
		ref_ptr<vulkan_logical_device> device = vulkan_context::get()->get_logical_device();

		vkEndCommandBuffer(a_command_buffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &a_command_buffer;

		vkQueueSubmit(device->get_graphics_queue_handle(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(device->get_graphics_queue_handle());

		vkFreeCommandBuffers(device->get_logical_device(), device->get_command_pool(), 1, &a_command_buffer);
	}

	// =================================================================
	// Vulkan Vertex Buffer
	// =================================================================
	vulkan_vertex_buffer::vulkan_vertex_buffer(void* a_data, uint32_t a_size, uint32_t a_element_count)
		: _buffer_size(a_size), _stored_data(a_data), _element_count(a_element_count)
	{
		create_vertex_buffer();
	}

	vulkan_vertex_buffer::~vulkan_vertex_buffer()
	{
		shutdown();
	}

	void vulkan_vertex_buffer::set_data(void* a_buffer, uint32_t a_size, uint32_t a_offset /*= 0*/)
	{

	}

	void vulkan_vertex_buffer::bind() const
	{

	}

	void vulkan_vertex_buffer::create_vertex_buffer()
	{
		ref_ptr<vulkan_logical_device> _device = vulkan_context::get()->get_logical_device();

		VkBuffer staging_buffer;
		VkDeviceMemory staging_memory;
		vulkan_buffer_util::create_buffer(vulkan_buffer_type::STAGING, staging_buffer, staging_memory, _buffer_size);

		void* dst_buffer;
		VK_CHECK_RESULT(vkMapMemory(_device->get_logical_device(), staging_memory, 0, _buffer_size, 0, &dst_buffer));
		memcpy(dst_buffer, _stored_data, (size_t)_buffer_size);
		vkUnmapMemory(_device->get_logical_device(), staging_memory);

		vulkan_buffer_util::create_buffer(vulkan_buffer_type::VERTEX, _vertex_buffer, _device_memory, _buffer_size);
		vulkan_buffer_util::copy_buffer(staging_buffer, _vertex_buffer, _buffer_size);

		vkDestroyBuffer(_device->get_logical_device(), staging_buffer, nullptr);
		vkFreeMemory(_device->get_logical_device(), staging_memory, nullptr);
	}

	void vulkan_vertex_buffer::shutdown()
	{
		ref_ptr<vulkan_logical_device> _device = vulkan_context::get()->get_logical_device();

		vkDestroyBuffer(_device->get_logical_device(), _vertex_buffer, nullptr);
		vkFreeMemory(_device->get_logical_device(), _device_memory, nullptr);
	}

	// =================================================================
	// Vulkan Index Buffer
	// =================================================================
	vulkan_index_buffer::vulkan_index_buffer(uint32_t a_size)
		: _buffer_size(a_size)
	{

	}

	vulkan_index_buffer::vulkan_index_buffer(void* a_data, uint32_t a_size)
		: _buffer_size(a_size), _stored_data(a_data)
	{
		create_index_buffer();
	}

	vulkan_index_buffer::~vulkan_index_buffer()
	{
		shutdown();
	}

	void vulkan_index_buffer::set_data(void* a_bufferm, uint32_t a_size, uint32_t a_offset /*= 0*/)
	{

	}

	void vulkan_index_buffer::bind() const
	{

	}

	void vulkan_index_buffer::shutdown()
	{
		ref_ptr<vulkan_logical_device> _device = vulkan_context::get()->get_logical_device();

		vkDestroyBuffer(_device->get_logical_device(), _index_buffer, nullptr);
		vkFreeMemory(_device->get_logical_device(), _device_memory, nullptr);
	}

	void vulkan_index_buffer::create_index_buffer()
	{
		ref_ptr<vulkan_logical_device> _device = vulkan_context::get()->get_logical_device();

		VkBuffer staging_buffer;
		VkDeviceMemory staging_memory;
		vulkan_buffer_util::create_buffer(vulkan_buffer_type::STAGING, staging_buffer, staging_memory, _buffer_size);

		void* dst_buffer;
		VK_CHECK_RESULT(vkMapMemory(_device->get_logical_device(), staging_memory, 0, _buffer_size, 0, &dst_buffer));
		memcpy(dst_buffer, _stored_data, (size_t)_buffer_size);
		vkUnmapMemory(_device->get_logical_device(), staging_memory);

		vulkan_buffer_util::create_buffer(vulkan_buffer_type::INDEX, _index_buffer, _device_memory, _buffer_size);
		vulkan_buffer_util::copy_buffer(staging_buffer, _index_buffer, _buffer_size);

		vkDestroyBuffer(_device->get_logical_device(), staging_buffer, nullptr);
		vkFreeMemory(_device->get_logical_device(), staging_memory, nullptr);
	}

}