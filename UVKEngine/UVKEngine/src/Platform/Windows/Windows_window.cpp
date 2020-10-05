/* Pre-comp header include
*/
#include "uvkpch.h"

/* Class header include
*/
#include "Windows_window.h"

/* Uciniti engine includes
*/
#include "Uciniti/Events/application_event.h"
#include "Uciniti/Events/key_event.h"
#include "Uciniti/Events/mouse_event.h"

namespace Uciniti
{
	static bool GLFW_initialised = false;

	static void GLFW_error_callback(int a_error, const char* a_description)
	{
		UVK_CORE_ERROR("GLFW Error ({0}): {1}", a_error, a_description);
	}

	window* window::create(const window_properties& a_props)
	{
		return new Windows_window(a_props);
	}

	Windows_window::Windows_window(const window_properties& a_props)
	{
		init(a_props);
	}

	Windows_window::~Windows_window()
	{
		shutdown();
	}

	void Windows_window::init(const window_properties& a_props)
	{
		data.title = a_props.title;
		data.width = a_props.width;
		data.height = a_props.height;

		UVK_CORE_INFO("Creating window {0} ({1}, {2})", data.title, data.width, data.height);

		if (!GLFW_initialised)
		{
			// TODO: glfwTerminate on system shutdown.
			int success = glfwInit();
			UVK_CORE_ASSERT(success, "Failed to intialise GLFW!");

			glfwSetErrorCallback(GLFW_error_callback);

			GLFW_initialised = true;
		}

		window_context = glfwCreateWindow((int)data.width, (int)data.height, data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window_context);
		glfwSetWindowUserPointer(window_context, &data);
		set_vsync(true);

		// Set GLFW callbacks.
		glfwSetWindowSizeCallback(window_context, [](GLFWwindow* a_window, int a_width, int a_height)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(a_window);
			data.width = a_width;
			data.height = a_height;

			window_resize_event new_event(a_width, a_height);
			data.event_callback(new_event);
		});

		glfwSetWindowCloseCallback(window_context, [](GLFWwindow* a_window)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(a_window);
			window_close_event new_event;
			data.event_callback(new_event);
		});

		glfwSetKeyCallback(window_context, [](GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(a_window);

			switch (a_action)
			{
				case GLFW_PRESS:
				{
					key_pressed_event new_event(a_key, 0);
					data.event_callback(new_event);
					break;
				}
				case GLFW_RELEASE:
				{
					key_released_event new_event(a_key);
					data.event_callback(new_event);
					break;
				}
				case GLFW_REPEAT:
				{
					key_pressed_event new_event(a_key, 1);
					data.event_callback(new_event);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(window_context, [](GLFWwindow* a_window, int a_button, int a_action, int a_mods)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(a_window);

			switch (a_action)
			{
				case GLFW_PRESS:
				{
					mouse_button_pressed_event new_event(a_button);
					data.event_callback(new_event);
					break;
				}
				case GLFW_RELEASE:
				{
					mouse_button_released_event new_event(a_button);
					data.event_callback(new_event);
					break;
				}
			}
		});

		glfwSetScrollCallback(window_context, [](GLFWwindow* a_window, double a_x_offset, double a_y_offset)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(a_window);
			
			mouse_scrolled_event new_event((float)a_x_offset, (float)a_y_offset);
			data.event_callback(new_event);
		});

		glfwSetCursorPosCallback(window_context, [](GLFWwindow* a_window, double a_x_pos, double a_y_pos)
		{
			window_data& data = *(window_data*)glfwGetWindowUserPointer(a_window);

			mouse_moved_event new_event((float)a_x_pos, (float)a_y_pos);
			data.event_callback(new_event);
		});
	}

	void Windows_window::shutdown()
	{
		glfwDestroyWindow(window_context);
	}

	void Windows_window::on_update()
	{
		glfwPollEvents();
		glfwSwapBuffers(window_context);
	}

	void Windows_window::set_vsync(bool a_enabled)
	{
		if (a_enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		data.vsync = a_enabled;
	}

	bool Windows_window::is_vsync() const
	{
		return data.vsync;
	}
}
