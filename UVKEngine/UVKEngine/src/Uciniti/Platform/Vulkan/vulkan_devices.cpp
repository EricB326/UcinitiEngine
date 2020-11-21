#include "uvkpch.h"

#include "vulkan_devices.h"

#include "vulkan_context.h"

namespace Uciniti
{
	// =================================================================
	// Vulkan Physical Device
	// =================================================================

	vulkan_physical_device::vulkan_physical_device(const VkSurfaceKHR& a_surface)
		: physical_device(VK_NULL_HANDLE), surface(a_surface)
	{
		printf("\n");
		UVK_CORE_INFO("Vulkan physical device initialisation...");

		VkInstance vulkan_instance = vulkan_context::get_instance();

		// Enumerate physical devices the instance can access.
		uint32_t physical_device_count = 0;
		vkEnumeratePhysicalDevices(vulkan_instance, &physical_device_count, nullptr);
		UVK_CORE_ASSERT(physical_device_count > 0, "");

		// Store the list of physical devices.
		std::vector<VkPhysicalDevice> physical_devices(physical_device_count);
		VK_CHECK_RESULT(vkEnumeratePhysicalDevices(vulkan_instance, &physical_device_count, physical_devices.data()));
	
		// Select the most suitable physical device.
		physical_device = select_physical_device(physical_devices);

		// Request and store the queue families.
		int requested_flags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT;
		queue_families = find_queue_families(requested_flags);

		// Store the supported physical device extensions.
		find_supported_extensions();

		find_depth_format();

		UVK_CORE_INFO("Vulkan physical device found successfully!");
	}

	vulkan_physical_device::~vulkan_physical_device()
	{

	}

	ref_ptr<vulkan_physical_device> vulkan_physical_device::select(const VkSurfaceKHR& a_surface)
	{
		return ref_ptr<vulkan_physical_device>(new vulkan_physical_device(a_surface));
	}

	bool vulkan_physical_device::is_extension_supported(const std::string& a_extension_name)
	{
		return supported_extensions.find(a_extension_name) != supported_extensions.end();
	}

	const uint32_t vulkan_physical_device::get_memory_type_index(uint32_t a_type_bits, VkMemoryPropertyFlags a_properties) const
	{
		// Iterate over all memory types available for the device used.
		for (uint32_t i = 0; i < physical_device_mem_properties.memoryTypeCount; i++)
		{
			if ((a_type_bits & 1) == 1)
			{
				if ((physical_device_mem_properties.memoryTypes[i].propertyFlags & a_properties) == a_properties)
					return i;
			}
			a_type_bits >>= 1;
		}

		UVK_CORE_ASSERT(false, "Could not find a suitable memory type!");
		return UINT32_MAX;
	}

	VkPhysicalDevice vulkan_physical_device::select_physical_device(const std::vector<VkPhysicalDevice>& a_physical_devices)
	{
		// #TODO: THIS FUNCTION NEEDS EXPANDING UPON.
		// Currently it just values discrete GPUs, if none are found then just take the first GPU available.
		// This is fine for the temporary development, but should be expanded upon with a scoring system.
		// The highest scoring GPU would become the selected device.

		VkPhysicalDevice selected_device = nullptr;

		// Get information about the current device being checked.
		for (const VkPhysicalDevice& this_physical_device : a_physical_devices)
		{
			vkGetPhysicalDeviceProperties(this_physical_device, &physical_device_properties);

			if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				selected_device = this_physical_device;
				break;
			}
		}

		// If no discrete GPU was found, just use the last found.
		// Selecting to avoid having another query to the devices properties.
		if (!selected_device)
		{
			UVK_CORE_TRACE("Could not locate a discrete GPU.");
			selected_device = a_physical_devices.back();
		}

		// If no GPU was found, the application cannot run.
		UVK_CORE_ASSERT(selected_device, "Could not find any physical devices!");

		// Store the information about the selected physical device.
		vkGetPhysicalDeviceFeatures(selected_device, &physical_device_features);
		vkGetPhysicalDeviceMemoryProperties(selected_device, &physical_device_mem_properties);

		// Return the selected device.
		UVK_CORE_TRACE("Physical device selected: '{0}'", physical_device_properties.deviceName);
		return selected_device;
	}

	queue_family_indices vulkan_physical_device::find_queue_families(const int& a_queue_flags)
	{
		// #TODO: This evaluation should occur when selecting a suitable physical device.
		// This would be to ensure that the physical device is capable of running the commands,
		// required by the application.

		queue_family_indices indices;

		// Get all queue family properties info for the given device.
		uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

		std::vector<VkQueueFamilyProperties> queue_family_list(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_family_list.data());

		UVK_CORE_TRACE("Queue familes located:");

		// Do an initial loop to populate each requested queue family with whatever is found first.
		int index = 0;
		for (const VkQueueFamilyProperties& this_queue_family : queue_family_list)
		{
			// If the queue families are populated, no need to continue looping.
			if (indices.is_valid())
				break;
			
			// Make sure there are actually queues to look over.
			if (this_queue_family.queueCount > 0)
			{
				// First set each queue bit type to a general purpose queue. Later check for dedicated queues.
				if (this_queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT && !indices.graphics_family.has_value())
				{
					indices.graphics_family = index; // If queue family is valid, then get the index.
					UVK_CORE_TRACE("\tGraphics queue family with index '{0}'", index);
				}

				if (a_queue_flags & VK_QUEUE_COMPUTE_BIT)
				{
					if (this_queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT && !indices.compute_family.has_value())
					{
						indices.compute_family = index; // If queue family is valid, then get the index.
						UVK_CORE_TRACE("\tCompute queue family with index '{0}'", index);
					}
				}

				if (a_queue_flags & VK_QUEUE_TRANSFER_BIT)
				{
					if (this_queue_family.queueFlags & VK_QUEUE_TRANSFER_BIT && !indices.transfer_family.has_value())
					{
						indices.transfer_family = index; // If queue family is valid, then get the index.
						UVK_CORE_TRACE("\tTransfer queue family with index '{0}'", index);
					}
				}

				// Check if the queue family supports presentation.
				VkBool32 present_support = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, index, surface, &present_support);

				if (present_support)
				{
					indices.present_family = index;
					UVK_CORE_TRACE("\tPresentation queue family with index '{0}'", index);
				}
			}
			index++;
		}

		// Do a second loop to try and populate queue family indices with dedicated queues.
		index = 0;
		for (const VkQueueFamilyProperties& this_queue_family : queue_family_list)
		{
			// Make sure there are actually queues to look over.
			if (this_queue_family.queueCount > 0)
			{
				// Attempt to find a dedicated queue for compute.
				// IE, supports compute but not graphics.
				if (a_queue_flags & VK_QUEUE_COMPUTE_BIT)
				{
					if (this_queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT && (this_queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0)
					{
						indices.compute_family = index;
						UVK_CORE_TRACE("\tDedicated compute queue family with index '{0}'", index);
					}
				}

				// Attempt to find a dedicated queue for transfer.
				// IE, supports transfer but not graphics and compute.
				if (a_queue_flags & VK_QUEUE_TRANSFER_BIT)
				{
					if (this_queue_family.queueFlags & VK_QUEUE_TRANSFER_BIT && (this_queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0 && (this_queue_family.queueFlags & VK_QUEUE_COMPUTE_BIT) == 0)
					{
						indices.transfer_family = index;
						UVK_CORE_TRACE("\tDedicated transfer queue family with index '{0}'", index);
					}
				}
			}
			index++;
		}

		return indices;
	}

	void vulkan_physical_device::find_supported_extensions()
	{
		uint32_t extension_count = 0;
		vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, nullptr);

		if (extension_count > 0)
		{
			std::vector<VkExtensionProperties> extensions(extension_count);
			vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &extension_count, &extensions.front());

			//UVK_CORE_TRACE("{0} extensions found:", extensions.size());
			for (const VkExtensionProperties& this_extension : extensions)
			{
				supported_extensions.emplace(this_extension.extensionName);
				//UVK_CORE_TRACE("\t'{0}'", this_extension.extensionName);
			}
		}
	}

	void vulkan_physical_device::find_depth_format()
	{
		// Since all depth formats may be optional, we need to find a suitable depth format to use,
		// start with the highest precision packed format.
		std::array<VkFormat, 5> depth_formats = {
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM
		};

		for (VkFormat this_format : depth_formats)
		{
			VkFormatProperties format_props;
			vkGetPhysicalDeviceFormatProperties(physical_device, this_format, &format_props);
			// Format must support depth stencil attachment for optimal tiling
			if (format_props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
			{
				_depth_format = this_format;
				return;
			}
		}

		_depth_format = VK_FORMAT_UNDEFINED;
	}

	// =================================================================
	// Vulkan Logical Device
	// =================================================================

	vulkan_logical_device::vulkan_logical_device(const ref_ptr<vulkan_physical_device> a_physical_device, const VkPhysicalDeviceFeatures& a_physical_device_features)
		: logical_device(VK_NULL_HANDLE), physical_device(a_physical_device), enable_debug_markers(false), enabled_features(a_physical_device_features)
	{
		printf("\n");
		UVK_CORE_INFO("Vulkan logical device initialisation...");

		// Information to create the logical device (sometimes referred to as "device").
		VkDeviceCreateInfo device_create_info(vk_base_device_create_info);

		// Create the queue family info.
		std::vector<VkDeviceQueueCreateInfo> queue_create_infos = create_queue_infos();
		device_create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
		device_create_info.pQueueCreateInfos = queue_create_infos.data();

		// Enabled physical device features.
		device_create_info.pEnabledFeatures = &enabled_features;

		// Physical device extensions the logical devices should used.
		std::vector<const char*> device_extension = {};

		// The device will be used for presenting to a display via a swapchain we need to request the swapchain extension.
		device_extension.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		if (!physical_device->is_extension_supported(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
			UVK_CORE_ASSERT(false, "Cannot run program without swap chain support!");

		// Enable the debug marker extension if it is present (likely meaning a debugging tool is present).
		if (physical_device->is_extension_supported(VK_EXT_DEBUG_MARKER_EXTENSION_NAME))
		{
			device_extension.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
			enable_debug_markers = true;
		}

		if (device_extension.size() > 0)
		{
			device_create_info.enabledExtensionCount = static_cast<uint32_t>(device_extension.size());
			device_create_info.ppEnabledExtensionNames = device_extension.data();
		}

		VK_CHECK_RESULT(vkCreateDevice(physical_device->get_physical_device(), &device_create_info, nullptr, &logical_device));

		create_queue_handles();
		create_command_pool();

		UVK_CORE_INFO("Vulkan logical device created successfully!");
	}

	std::vector<VkDeviceQueueCreateInfo> vulkan_logical_device::create_queue_infos()
	{
		queue_family_indices indices = physical_device->get_queue_family_indices();
		std::vector<VkDeviceQueueCreateInfo> queue_create_infos{};

		const float default_queue_priority(0.0f);

		UVK_CORE_TRACE("Creating queue family infos:");

		// Graphics queue creation.
		if (indices.graphics_family.has_value())
		{
			VkDeviceQueueCreateInfo queue_info(vk_base_device_queue_create_info);
			queue_info.queueFamilyIndex = indices.graphics_family.value();
			queue_info.queueCount = 1;
			queue_info.pQueuePriorities = &default_queue_priority;
			queue_create_infos.push_back(queue_info);
			UVK_CORE_TRACE("\tGraphics queue family created.");
		}

		// Present queue creation.
		if (indices.present_family.has_value())
		{
			if (indices.present_family.value() != indices.graphics_family.value())
			{
				VkDeviceQueueCreateInfo queue_info(vk_base_device_queue_create_info);
				queue_info.queueFamilyIndex = indices.present_family.value();
				queue_info.queueCount = 1;
				queue_info.pQueuePriorities = &default_queue_priority;
				queue_create_infos.push_back(queue_info);
				UVK_CORE_TRACE("\tPresentation queue family created.");
			}
		}

		// Dedicated compute queue creation.
		if (indices.compute_family.has_value())
		{
			if (indices.compute_family.value() != indices.graphics_family.value())
			{
				// If compute family index differs, we need an additional queue create info for the compute queue.
				VkDeviceQueueCreateInfo queue_info(vk_base_device_queue_create_info);
				queue_info.queueFamilyIndex = indices.compute_family.value();
				queue_info.queueCount = 1;
				queue_info.pQueuePriorities = &default_queue_priority;
				queue_create_infos.push_back(queue_info);
				UVK_CORE_TRACE("\tCompute queue family created.");
			}
		}

		// Dedicated transfer queue creation.
		if (indices.transfer_family.has_value())
		{
			if ((indices.transfer_family.value() != indices.graphics_family.value()) && (indices.transfer_family.value() != indices.compute_family.value()))
			{
				// If transfer family index differs, we need an additional queue create info for the transfer queue.
				VkDeviceQueueCreateInfo queue_info(vk_base_device_queue_create_info);
				queue_info.queueFamilyIndex = indices.transfer_family.value();
				queue_info.queueCount = 1;
				queue_info.pQueuePriorities = &default_queue_priority;
				queue_create_infos.push_back(queue_info);
				UVK_CORE_TRACE("\tTransfer queue family created.");
			}
		}

		UVK_CORE_TRACE("\tPresentation queue family created.");

		return queue_create_infos;
	}

	void vulkan_logical_device::create_queue_handles()
	{
		UVK_CORE_TRACE("Finding device queues:");

		vkGetDeviceQueue(logical_device, physical_device->get_queue_family_indices().graphics_family.value(), 0, &graphics_queue);
		UVK_CORE_TRACE("\tGraphic queue handle found.");

		vkGetDeviceQueue(logical_device, physical_device->get_queue_family_indices().present_family.value(), 0, &present_queue);
		UVK_CORE_TRACE("\tPresentation queue handle found.");
	}

	void vulkan_logical_device::create_command_pool()
	{
		VkCommandPoolCreateInfo command_pool_info(vk_base_command_pool_create_info);
		command_pool_info.queueFamilyIndex = physical_device->get_queue_family_indices().graphics_family.value();
		command_pool_info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		VK_CHECK_RESULT(vkCreateCommandPool(logical_device, &command_pool_info, nullptr, &command_pool));
		UVK_CORE_TRACE("Command pool created.");
	}

	void vulkan_logical_device::shutdown()
	{
		vkDestroyCommandPool(logical_device, command_pool, nullptr);
		vkDestroyDevice(logical_device, nullptr);
	}

}