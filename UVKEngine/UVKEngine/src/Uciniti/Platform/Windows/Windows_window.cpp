/* Pre-comp header include
*/
#include "uvkpch.h"

/* Class header include
*/
#include "Windows_window.h"

/* Uciniti engine includes
*/

namespace Uciniti
{
	static bool GLFW_initialised = false;

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

			GLFW_initialised = true;
		}

		window_context = glfwCreateWindow((int)data.width, (int)data.height, data.title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window_context);
		glfwSetWindowUserPointer(window_context, &data);
		set_vsync(true);
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
