#ifndef WINDOWS_WINDOW_H
#define WINDOWS_WINDOW_H

/* Vendor includes
*/
#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

/* Uciniti engine includes
*/
#include "Uciniti/Core/window.h"
#include "Uciniti/Renderer/renderer_context.h"

namespace Uciniti
{
	class Windows_window : public window
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		Windows_window(const window_properties& a_props);
		virtual ~Windows_window();

		/* @brief Polls input events, buffers, and other GLFW related events.
				  Ran once per frame.
		*/
		void on_update() override;

		inline uint16_t get_width() const override { return data.width; }
		inline uint16_t get_height() const override { return data.height; }
		//virtual ref<renderer_context> get_render_context() const override { return render_context; }

		inline void set_event_callback(const event_callback_fn& a_callback) override { data.event_callback = a_callback; }
		void set_vsync(bool a_enabled) override;
		bool is_vsync() const override;

		inline virtual void* get_native_window() const override { return window_context; }


	private:
		/***************************************************************/
		// Private Functions
		/***************************************************************/

		virtual void init(const window_properties& a_props);

		virtual void shutdown();


		/***************************************************************/
		// Private Variables
		/***************************************************************/
		/* @brief Window creation context using GLFW.
		*/
		GLFWwindow* window_context;

		//ref<renderer_context> render_context;
		renderer_context* render_context;

		/* @brief Stores the data that may be requested by GLFW event callbacks.
		*/
		struct window_data
		{
			std::string title;
			uint16_t width, height;
			bool vsync;

			event_callback_fn event_callback;
		};
		
		/* @brief Creation of the window data struct.
		*/
		window_data data;
	};
}

#endif // !WINDOWS_WINDOW_H