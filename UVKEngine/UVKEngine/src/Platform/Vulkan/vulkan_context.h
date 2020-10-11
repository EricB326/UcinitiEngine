#ifndef VULKAN_CONTEXT_H
#define VULKAN_CONTEXT_H

#include "GLFW/glfw3.h"

#include "Uciniti/Core/base.h"
#include "vulkan_base.h" 
#include "vulkan_validation.h"
#include "vulkan_devices.h"
#include "vulkan_allocator.h"
#include "vulkan_swap_chain.h"

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

		//vulkan_allocator allocator;

		// Devices.
		ref<vulkan_physical_device> physical_device;
		ref<vulkan_logical_device> logical_device;

		// Swap chain.
		vulkan_swap_chain swap_chain;

		void create_instance();
		void select_physical_device();
		void create_logical_device();
		void create_allocator();
		void create_swap_chain();

		std::vector<const char*> get_instance_extensions();
		bool instance_extension_support(const std::vector<const char*>& a_required_extensions);

		void shutdown();
	};
}

#endif // VULKAN_CONTEXT_H