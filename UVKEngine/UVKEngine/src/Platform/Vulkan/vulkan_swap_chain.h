#ifndef VULKAN_SWAP_CHAIN_H
#define VULKAN_SWAP_CHAIN_H

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "vulkan_devices.h"

namespace Uciniti
{

	/* @brief Properties needed to ensure the surface and swap chain will work.
	*/
	struct swap_chain_details
	{
		VkSurfaceCapabilitiesKHR surface_capabilities;		 // Surface properties, e.g. image size/extent.
		std::vector<VkSurfaceFormatKHR> surface_formats;	 // Surface image formats, e.g. RGBA and size of each colour.
		std::vector<VkPresentModeKHR> surface_present_modes; // How images should be presented to the window/screen.
	};

	struct swap_chain_image_buffer
	{
		VkImage image;
		VkImageView image_view;
	};

	class vulkan_swap_chain
	{
	public:
		vulkan_swap_chain() = default;
		//vulkan_swap_chain(GLFWwindow* a_window_context, const ref<vulkan_logical_device>& a_logical_device);

		void create_surface(GLFWwindow* a_window_context, const VkInstance& a_vulkan_instance);
		void create_swap_chain(const ref<vulkan_logical_device>& a_logical_device);

		const VkSurfaceKHR& get_surface() const { return surface; }

		void shutdown();

	private:
		GLFWwindow* window_context;
		VkInstance vulkan_instance;
		ref<vulkan_logical_device> logical_device;

		VkSurfaceKHR surface;
		VkSwapchainKHR swap_chain;
		swap_chain_details swap_chain_support_details;
		std::vector<swap_chain_image_buffer> swap_chain_buffer;
		std::vector<VkImage> swap_chain_images;

		VkSurfaceFormatKHR select_optimal_surface_format();
		VkPresentModeKHR select_best_present_mode();
		VkExtent2D select_swap_chain_extents(const VkSurfaceCapabilitiesKHR& a_surface_capabilities);
	};
}

#endif // !VULKAN_SWAP_CHAIN_H