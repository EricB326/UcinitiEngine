#include "uvkpch.h"
#include "vulkan_swap_chain.h"

#undef min
#undef max 
#include <cstdint>

namespace Uciniti
{
	void vulkan_swap_chain::create_surface(GLFWwindow* a_window_context, const VkInstance& a_vulkan_instance)
	{
		window_context = a_window_context;
		vulkan_instance = a_vulkan_instance;

		printf("\n");
		UVK_CORE_INFO("Vulkan surface initialisation...");
		/* What the glfwCreateWindowSurface() does behind the scenes (Windows OS):

			  VkWin32SurfaceCreateInfoKHR createInfo{};
			  createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			  createInfo.hwnd = glfwGetWin32Window(window);
			  createInfo.hinstance = GetModuleHandle(nullptr);
			  vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface
		*/

		VK_CHECK_RESULT(glfwCreateWindowSurface(vulkan_instance, window_context, nullptr, &surface));
		UVK_CORE_INFO("Vulkan surface created successfully!");
	}

	void vulkan_swap_chain::create_swap_chain(const ref<vulkan_logical_device>& a_logical_device)
	{
		printf("\n");
		UVK_CORE_INFO("Vulkan swap chain initialisation...");
		logical_device = a_logical_device;

		VkSwapchainKHR old_swap_chain = VK_NULL_HANDLE;

		// Get the surface capabilities.
		VkSurfaceCapabilitiesKHR surface_capabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(logical_device->get_physical_device_ref()->get_physical_device(), surface, &surface_capabilities);

		// Select optimal surface values for the swap chain.
		VkSurfaceFormatKHR surface_format = select_optimal_surface_format();

		VkPresentModeKHR present_mode = select_best_present_mode();
		UVK_CORE_TRACE("\tUsing presentation mode '{0}'", present_mode);

		VkExtent2D swap_chain_extents = select_swap_chain_extents(surface_capabilities);
		UVK_CORE_TRACE("\tSwap chain extents '{0}:{1}'", swap_chain_extents.width, swap_chain_extents.height);

		// How many images are in the swap chain. Get one more than the min to allow triple buffer.
		uint32_t image_count = surface_capabilities.minImageCount + 1;

		// If the image count is higher than max images allowed, then clamp it down to max.
		// If it is zero, then the max image count is limitless.
		if (surface_capabilities.maxImageCount > 0 && surface_capabilities.maxImageCount < image_count)
			image_count = surface_capabilities.maxImageCount;

		// Find the transformation of the surface.
		VkSurfaceTransformFlagsKHR pre_transform;
		if (surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
			pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR; // We prefer a non-rotated transform.
		else
			pre_transform = surface_capabilities.currentTransform;

		// Find a supported composite alpha format (not all devices support alpha opaque).
		VkCompositeAlphaFlagBitsKHR composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		// Simply select the first composite alpha format available.
		std::vector<VkCompositeAlphaFlagBitsKHR> composite_alpha_flags = 
		{
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
		};

		for (auto& composite_alpha_flag : composite_alpha_flags) 
		{
			if (surface_capabilities.supportedCompositeAlpha & composite_alpha_flag) 
			{
				composite_alpha = composite_alpha_flag;
				break;
			};
		}

		// Creation information for the swap chain.
		VkSwapchainCreateInfoKHR swap_chain_create_info(vk_base_swap_chain_create_info);
		swap_chain_create_info.surface = surface;											// Specify the surface.
		swap_chain_create_info.minImageCount = image_count;									// Minimum images in swapchain.
		swap_chain_create_info.imageFormat = surface_format.format;							// Swapchain format.
		swap_chain_create_info.imageColorSpace = surface_format.colorSpace;					// Swapchain colour space.
		swap_chain_create_info.imageExtent = swap_chain_extents;							// Swapchain image extents.
		swap_chain_create_info.imageArrayLayers = 1;										// Number of layers for each image in chain.
		swap_chain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;			// What attachment images will be used as.
		swap_chain_create_info.presentMode = present_mode;									// Swapchain presentation mode.
		swap_chain_create_info.preTransform = (VkSurfaceTransformFlagBitsKHR)pre_transform;	// Transform to perform on swap chain images.
		swap_chain_create_info.compositeAlpha = composite_alpha;							// How to handle blending images with external graphics (e.g. other windows).
		swap_chain_create_info.clipped = VK_TRUE;											// Whether to clip parts of image not in view (e.g. behind another window, off screen, etc).
		
		// If the presentation family is different to another queue, than swap chain must let images be shared between families.
		if (logical_device->get_physical_device_ref()->get_queue_family_indices().graphics_family.value() != logical_device->get_physical_device_ref()->get_queue_family_indices().present_family.value())
		{
			// Queues to share between.
			uint32_t queue_family_indices[] =
			{
				(uint32_t)logical_device->get_physical_device_ref()->get_queue_family_indices().graphics_family.value(),
				(uint32_t)logical_device->get_physical_device_ref()->get_queue_family_indices().present_family.value()
			};

			swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Image share handling is constantly occurring.
			swap_chain_create_info.queueFamilyIndexCount = 2;					  // Number of queues to share images between.
			swap_chain_create_info.pQueueFamilyIndices = queue_family_indices;	  // Array of queues to share between.
		}
		else
		{
			swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swap_chain_create_info.queueFamilyIndexCount = 0;
			swap_chain_create_info.pQueueFamilyIndices = nullptr;
		}

		// Enable transfer source on swap chain images if supported.
		if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
			swap_chain_create_info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}

		// Enable transfer destination on swap chain images if supported.
		if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
			swap_chain_create_info.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}

		// If old swap chain been destroyed and this one replaces it, then link old one to quickly hand over responsibilities.
		swap_chain_create_info.oldSwapchain = VK_NULL_HANDLE;

		VK_CHECK_RESULT(vkCreateSwapchainKHR(logical_device->get_logical_device(), &swap_chain_create_info, nullptr, &swap_chain));
		UVK_CORE_INFO("Vulkan swap chain created successfully!");

		// If an existing swap chain is re-created, destroy the old swap chain.
		// This also cleans up all the presentable images.
		if (old_swap_chain != VK_NULL_HANDLE)
		{
			for (uint32_t i = 0; i < image_count; i++)
			{
				vkDestroyImageView(logical_device->get_logical_device(), swap_chain_buffer[i].image_view, nullptr);
			}
			vkDestroySwapchainKHR(logical_device->get_logical_device(), old_swap_chain, nullptr);
		}

		// Get swap chain images.
		create_image_views(image_count, surface_format);

	}

	void vulkan_swap_chain::shutdown()
	{
		for (swap_chain_image_buffer& this_image : swap_chain_buffer)
		{
			vkDestroyImageView(logical_device->get_logical_device(), this_image.image_view, nullptr);
		}

		vkDestroySwapchainKHR(logical_device->get_logical_device(), swap_chain, nullptr);

		vkDestroySurfaceKHR(vulkan_instance, surface, nullptr);
	}

	VkSurfaceFormatKHR vulkan_swap_chain::select_optimal_surface_format()
	{
		UVK_CORE_TRACE("Finding optimal surface format and colour space...");
		// Get available formats.
		std::vector<VkSurfaceFormatKHR> surface_formats;
		uint32_t format_count = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(logical_device->get_physical_device_ref()->get_physical_device(), surface, &format_count, nullptr);

		// Validate that there was formates returned and store them.
		if (format_count)
		{
			surface_formats.resize(format_count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(logical_device->get_physical_device_ref()->get_physical_device(), surface, &format_count, surface_formats.data());
		}
		else
			UVK_CORE_ASSERT(false, "No available surface formats!");

		// If only one format and the format is undefined, it actually means ALL format is available.
		// Just select the preferred format because of this.
		if (surface_formats.size() == 1 && surface_formats[0].format)
		{
			UVK_CORE_TRACE("\tUsing surface format 'R8B8G8A8 UNORM'");
			UVK_CORE_TRACE("\tUsing surface colour space 'SRGB NONLINEAR'");
			return { VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		}

		// Loop through each format and find one that suits our combination.
		for (const VkSurfaceFormatKHR& this_surface_format : surface_formats)
		{
			// See if this format has the main formate, or backup, as well as the colour space preferred.
			if ((this_surface_format.format == VK_FORMAT_R8G8B8A8_UNORM || this_surface_format.format == VK_FORMAT_B8G8R8A8_UNORM) && this_surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				if (this_surface_format.format == VK_FORMAT_R8G8B8A8_UNORM)
					UVK_CORE_TRACE("\tUsing surface format 'R8B8G8A8 UNORM'");
				else if (this_surface_format.format == VK_FORMAT_B8G8R8A8_UNORM)
					UVK_CORE_TRACE("\tUsing surface format 'B8G8R8A8 UNORM'");

				UVK_CORE_TRACE("\tUsing surface colour space 'SRGB NONLINEAR'");
				return this_surface_format;
			}
		}

		// If can't find optimal format, then just return the first format.
		return surface_formats[0];
	}

	VkPresentModeKHR vulkan_swap_chain::select_best_present_mode()
	{
		UVK_CORE_TRACE("Finding optimal presentation mode.");

		// Get available presentation modes.
		std::vector<VkPresentModeKHR> surface_present_modes;
		uint32_t presentation_count = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(logical_device->get_physical_device_ref()->get_physical_device(), surface, &presentation_count, nullptr);

		// Store the presentation modes.
		if (presentation_count)
		{
			surface_present_modes.resize(presentation_count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(logical_device->get_physical_device_ref()->get_physical_device(), surface, &presentation_count, surface_present_modes.data());
		}
		else
			UVK_CORE_ASSERT(false, "No available surface presentation modes!");

		// Loop through each presentation mode to find optimal presentation mode.
		for (const VkPresentModeKHR& this_presentation_mode : surface_present_modes)
		{
			// If the Mailbox presentation mode is found, return it.
			if (this_presentation_mode == VK_PRESENT_MODE_MAILBOX_KHR)
				return this_presentation_mode;
		}

		// If mailbox mode is unable to be found, go with FIFO.
		// As part of the Vulkan spec, this always is available.
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D vulkan_swap_chain::select_swap_chain_extents(const VkSurfaceCapabilitiesKHR& a_surface_capabilities)
	{
		UVK_CORE_TRACE("Setting swap chain extents.");
		// If current extent is at the numeric limits, then extent can vary. Otherwise, it is size of the window.
		if (a_surface_capabilities.currentExtent.width != UINT32_MAX)
			return a_surface_capabilities.currentExtent;
		else
		{
			// If the value can vary, need to set manually.
			// Get window size.
			int width, height;
			glfwGetFramebufferSize(window_context, &width, &height);

			// Create new extent using window size.
			VkExtent2D new_extents = {};
			new_extents.width = static_cast<uint32_t>(width);
			new_extents.height = static_cast<uint32_t>(height);

			// Surface also defines max and min, make sure within boundaries by clamping value.
			new_extents.width = std::max(a_surface_capabilities.minImageExtent.width, std::min(a_surface_capabilities.maxImageExtent.width, new_extents.width));
			new_extents.height = std::max(a_surface_capabilities.minImageExtent.height, std::min(a_surface_capabilities.maxImageExtent.height, new_extents.height));

			return new_extents;
		}
	}

	void vulkan_swap_chain::create_image_views(uint32_t image_count, VkSurfaceFormatKHR& surface_format)
	{
		vkGetSwapchainImagesKHR(logical_device->get_logical_device(), swap_chain, &image_count, nullptr);
		swap_chain_images.resize(image_count);
		vkGetSwapchainImagesKHR(logical_device->get_logical_device(), swap_chain, &image_count, swap_chain_images.data());

		// Get the swap chain buffers containing the image and image view.
		swap_chain_buffer.resize(image_count);
		for (uint32_t i = 0; i < image_count; i++)
		{
			VkImageViewCreateInfo image_view_create_info(vk_base_image_view_create_info);
			image_view_create_info.format = surface_format.format;
			image_view_create_info.components =
			{
				VK_COMPONENT_SWIZZLE_R,
				VK_COMPONENT_SWIZZLE_G,
				VK_COMPONENT_SWIZZLE_B,
				VK_COMPONENT_SWIZZLE_A
			};

			image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_create_info.subresourceRange.baseMipLevel = 0;
			image_view_create_info.subresourceRange.levelCount = 1;
			image_view_create_info.subresourceRange.baseArrayLayer = 0;
			image_view_create_info.subresourceRange.layerCount = 1;
			image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D; // Type of image (1D, 2D, 3D, Cube, etc).
			//image_view_create_info.flags = 0;

			swap_chain_buffer[i].image = swap_chain_images[i];
			image_view_create_info.image = swap_chain_buffer[i].image;

			VK_CHECK_RESULT(vkCreateImageView(logical_device->get_logical_device(), &image_view_create_info, nullptr, &swap_chain_buffer[i].image_view));
		}
	}

}