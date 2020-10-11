#ifndef VULKAN_SWAP_CHAIN_H
#define VULKAN_SWAP_CHAIN_H

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "vulkan_devices.h"

namespace Uciniti
{
	class vulkan_swap_chain
	{
	public:
		vulkan_swap_chain() = default;
		//vulkan_swap_chain(GLFWwindow* a_window_context, const ref<vulkan_logical_device>& a_logical_device);

		void create(GLFWwindow* a_window_context, const VkInstance& a_vulkan_instance, const ref<vulkan_logical_device>& a_logical_device);
		void shutdown();

	private:
		GLFWwindow* window_context;
		VkInstance vulkan_instance;
		ref<vulkan_logical_device> logical_device;

		VkSurfaceKHR surface;
		VkSwapchainKHR swap_chain;

		void create_surface();
		void create_swap_chain();


	};
}

#endif // !VULKAN_SWAP_CHAIN_H