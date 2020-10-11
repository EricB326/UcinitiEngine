#ifndef VULKAN_ALLOCATOR_H
#define VULKAN_ALLOCATOR_H

#include "vulkan_base.h"
#include "vulkan_devices.h"

namespace Uciniti
{
	class vulkan_allocator
	{
	public:
		vulkan_allocator() = default;
		vulkan_allocator(const scope<vulkan_logical_device>& a_logical_device, const std::string& a_tag = "");
		~vulkan_allocator();

		void allocate_memory(VkMemoryRequirements a_requirements, VkDeviceMemory a_dest, VkMemoryPropertyFlags a_flags);


	private:
		scope<vulkan_logical_device> logical_device;
		std::string tag;

	};
}

#endif // !VULKAN_ALLOCATOR_H