#ifndef VULKAN_DEVICES_H
#define VULKAN_DEVICES_H

#include "vulkan_base.h"

namespace Uciniti
{
	class vulkan_physical_device
	{
	public:
		vulkan_physical_device();
		~vulkan_physical_device();

		static scope<vulkan_physical_device> create();

	private:
		VkPhysicalDevice physical_device;

	};

	class vulkan_logical_device
	{
	public:
		vulkan_logical_device();
		~vulkan_logical_device();

	private:
		VkDevice logical_device;

	};
}

#endif // !VULKAN_DEVICES_H