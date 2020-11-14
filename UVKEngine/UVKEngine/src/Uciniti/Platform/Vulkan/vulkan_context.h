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

		/* @brief There is no copy constructor. If the compiler would otherwise
		  generate a copy call, it will instead fail.
		*/
		vulkan_context(const vulkan_context& a_other) = delete;

		/* @brief There is no assignment allowed.
		*/
		vulkan_context& operator=(const vulkan_context& a_other) = delete;

		virtual ~vulkan_context();


		virtual void init() override;
		virtual void begin_frame() override;
		virtual void swap_buffers() override;

		vulkan_logical_device* get_logical_device() { return logical_device; }
		vulkan_swap_chain& get_swap_chain() { return swap_chain; }

		static VkInstance get_instance() { return vulkan_instance; }
		static ref_ptr<vulkan_context> get() { return (std::static_pointer_cast<vulkan_context>(renderer::get_context())); }
	
	private:
		GLFWwindow* window_handle;

		inline static VkInstance vulkan_instance;
		std::unique_ptr<vulkan_validation> validation_handle;

		//vulkan_allocator allocator;

		// Devices.
		//ref<vulkan_physical_device> physical_device;
		//ref<vulkan_logical_device> logical_device;
		vulkan_physical_device* physical_device;
		vulkan_logical_device* logical_device;

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