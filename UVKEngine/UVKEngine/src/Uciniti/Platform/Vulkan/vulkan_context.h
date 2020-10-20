#ifndef VULKAN_CONTEXT_H
#define VULKAN_CONTEXT_H

#include "Uciniti/Core/base.h"
#include "Uciniti/Renderer/renderer.h"
#include "vulkan_base.h" 
#include "vulkan_validation.h"
#include "vulkan_devices.h"
#include "vulkan_allocator.h"
#include "vulkan_swap_chain.h"
#include "vulkan_shader.h"

struct GLFWwindow;

namespace Uciniti
{
	class vulkan_context : public renderer_context
	{
	public:
		vulkan_context(GLFWwindow* a_window_handle);
		virtual ~vulkan_context();

		virtual void create() override;
		virtual void swap_buffers() override;

		//static ref<vulkan_logical_device> get_logical_device() { return logical_device; }

		static VkInstance get_instance() { return vulkan_instance; }
		//static ref<vulkan_context> get() { return ref<vulkan_context>(renderer::get_context()); }
		//static ref<vulkan_logical_device> get_current_device() { return get()->get_logical_device(); }
	
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
		void create_surface();
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