#include "uvkpch.h"
#include "vulkan_swap_chain.h"

namespace Uciniti
{

	void vulkan_swap_chain::create(GLFWwindow* a_window_context, const VkInstance& a_vulkan_instance, const ref<vulkan_logical_device>& a_logical_device)
	{
		window_context = a_window_context;
		vulkan_instance = a_vulkan_instance;
		logical_device = a_logical_device;

		printf("\n");
		UVK_CORE_INFO("Vulkan swap chain initialisation...");
		create_surface();
	}

	void vulkan_swap_chain::create_surface()
	{
		/* What the glfwCreateWindowSurface() does behind the scenes (windows):

			  VkWin32SurfaceCreateInfoKHR createInfo{};
			  createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			  createInfo.hwnd = glfwGetWin32Window(window);
			  createInfo.hinstance = GetModuleHandle(nullptr);
			  vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface
		*/

		VK_CHECK_RESULT(glfwCreateWindowSurface(vulkan_instance, window_context, nullptr, &surface));
		UVK_CORE_TRACE("Vulkan surface created.");
	}

	void vulkan_swap_chain::shutdown()
	{
		vkDestroySurfaceKHR(vulkan_instance, surface, nullptr);
	}

}