#include "uvkpch.h"

#include "vulkan_allocator.h"

namespace Uciniti
{

	vulkan_allocator::vulkan_allocator(const scope<vulkan_logical_device>& a_logical_device, const std::string& a_tag)
		: logical_device(a_logical_device.get()), tag(a_tag)
	{

	}

	vulkan_allocator::~vulkan_allocator()
	{

	}

	void vulkan_allocator::allocate_memory(VkMemoryRequirements a_requirements, VkDeviceMemory a_dest, VkMemoryPropertyFlags a_flags)
	{
		UVK_CORE_ASSERT(logical_device, "");

		UVK_CORE_TRACE("vulkan allocator ({0}): allocating {1} bytes.", tag, a_requirements.size);

		VkMemoryAllocateInfo mem_alloc(vk_base_memory_alloc_info);
		mem_alloc.allocationSize = a_requirements.size;
		mem_alloc.memoryTypeIndex = logical_device->get_physical_device_ref()->get_memory_type_index(a_requirements.memoryTypeBits, a_flags);

		VK_CHECK_RESULT(vkAllocateMemory(logical_device->get_logical_device(), &mem_alloc, nullptr, &a_dest));
	}

}