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
		void create_swap_chain(const vulkan_logical_device* a_logical_device, const uint32_t& a_width, const uint32_t& a_height);

		void begin_frame();
		void present();

		void on_resize(const uint32_t& a_width, const uint32_t& a_height);

		const VkSurfaceKHR& get_surface() const { return surface; }
		const uint32_t get_width() const { return width; }
		const uint32_t get_height() const { return height; }
		const uint32_t get_image_count() const { return image_count; }
		const uint32_t get_current_image() const { return _current_buffer_index; }

		const VkRenderPass get_render_pass() const { return _render_pass; }

		const VkCommandBuffer get_current_draw_command_buffer() { return get_draw_command_buffer(_current_buffer_index); }

		const VkFramebuffer get_framebuffer(uint32_t a_index) const
		{
			UVK_CORE_ASSERT(a_index < image_count, "");
			return framebuffers[a_index];
		}

		const VkCommandBuffer get_draw_command_buffer(uint32_t a_index) const
		{
			UVK_CORE_ASSERT(a_index < image_count, "");
			return draw_command_buffer[a_index];
		}

		const VkCommandPool get_command_pool() const { return command_pool; }

		void shutdown();

	private:
		VkInstance vulkan_instance;
		const vulkan_logical_device* logical_device;
		GLFWwindow* window_context;

		// Colour format and colour space.
		VkSurfaceFormatKHR surface_format;

		VkSwapchainKHR swap_chain = VK_NULL_HANDLE;
		std::vector<VkImage> swap_chain_images;

		std::vector<swap_chain_image_buffer> swap_chain_buffer;

		//swap_chain_details swap_chain_support_details;

		// #TODO: Move render pass creation out of the swap chain? Perhaps do what I did in my first Vulkan renderer.
		//		  Even just making its own class to create render passes uniquely.
		VkRenderPass _render_pass;

		uint32_t image_count;
		uint32_t width = 0, height = 0;

		VkSurfaceKHR surface;

		VkCommandPool command_pool;
		std::vector<VkCommandBuffer> draw_command_buffer;

		struct
		{
			// Swap chain semaphore.
			VkSemaphore present_complete;
			// Command buffer semaphore.
			VkSemaphore render_complete;
		} _semaphores;
		VkSubmitInfo _submit_info;
		std::vector<VkFence> _wait_fences;

		std::vector<VkFramebuffer> framebuffers;

		uint32_t _current_buffer_index = 0;

		void acquire_next_image(VkSemaphore a_semaphore, uint32_t* a_image_index);
		VkResult request_presentation(VkQueue a_queue, uint32_t a_image_index, VkSemaphore a_signal_semaphore);

		VkSurfaceFormatKHR select_optimal_surface_format();
		VkPresentModeKHR select_best_present_mode();
		VkExtent2D select_swap_chain_extents(const VkSurfaceCapabilitiesKHR& a_surface_capabilities);
		void create_image_views(uint32_t image_count, VkSurfaceFormatKHR& surface_format);
		void create_command_buffers();
		void create_sync_objects();
		void create_render_pass();
		void create_framebuffers();

		void destroy_old_swap_chain();
	};
}

#endif // !VULKAN_SWAP_CHAIN_H