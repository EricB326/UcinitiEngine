#include "uvkpch.h"

#include "vulkan_devices.h"

#include "vulkan_context.h"

namespace Uciniti
{
	vulkan_physical_device::vulkan_physical_device()
	{
		VkInstance vulkan_instance = vulkan_context::get_instance();

		// Enumerate physical devices the instance can access.
		uint32_t physical_device_count = 0;
		vkEnumeratePhysicalDevices(vulkan_instance, &physical_device_count, nullptr);
		UVK_CORE_ASSERT(physical_device_count > 0, "");

		// Store the list of physical devices.
		std::vector<VkPhysicalDevice> physical_device_list(physical_device_count);
		VK_CHECK_RESULT(vkEnumeratePhysicalDevices(vulkan_instance, &physical_device_count, physical_device_list.data()));
	}

	vulkan_physical_device::~vulkan_physical_device()
	{

	}

	scope<vulkan_physical_device> vulkan_physical_device::create()
	{

	}

	// =====

	vulkan_logical_device::vulkan_logical_device()
	{

	}

	vulkan_logical_device::~vulkan_logical_device()
	{

	}
}