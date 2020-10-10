#ifndef VULKAN_CONTEXT_H
#define VULKAN_CONTEXT_H

#include "GLFW/glfw3.h"

#include "vulkan_base.h" 
#include "vulkan_validation.h"
#include "vulkan_devices.h"

struct GLFWwindow;

namespace Uciniti
{
	class vulkan_context
	{
	public:
		vulkan_context(GLFWwindow* a_window_handle);
		virtual ~vulkan_context();

		void create();
		void swap_buffers();

		static VkInstance get_instance() { return vulkan_instance; }

	private:
		GLFWwindow* window_handle;

		inline static VkInstance vulkan_instance;
		std::unique_ptr<vulkan_validation> validation_handle;
		scope<vulkan_physical_device> physical_device;
		scope<vulkan_logical_device> logical_device;

		void create_instance();
		void create_physical_device();
		void create_logical_device();

		std::vector<const char*> get_instance_extensions();
		bool instance_extension_support(const std::vector<const char*>& a_required_extensions);

		void shutdown();
	};
}

#endif // VULKAN_CONTEXT_H