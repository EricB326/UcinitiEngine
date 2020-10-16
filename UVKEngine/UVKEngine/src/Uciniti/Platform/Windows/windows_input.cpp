#include "uvkpch.h"

#include <GLFW/glfw3.h>

#include "Uciniti/Core/input.h"
#include "Windows_window.h"
#include "Uciniti/Core/application.h"


namespace Uciniti
{
	bool input::is_key_pressed(key_code a_keycode)
	{
		auto& window = static_cast<Windows_window&>(application::get().get_window());
		auto state = glfwGetKey(static_cast<GLFWwindow*>(window.get_native_window()), static_cast<int32_t>(a_keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool input::is_mouse_button_pressed(int a_button)
	{
		auto& window = static_cast<Windows_window&>(application::get().get_window());
		auto state = glfwGetMouseButton(static_cast<GLFWwindow*>(window.get_native_window()), a_button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> input::get_mouse_pos()
	{
		auto window = static_cast<GLFWwindow*>(application::get().get_window().get_native_window());
		double x_pos, y_pos;
		glfwGetCursorPos(window, &x_pos, &y_pos);
		return { (float)x_pos, (float)y_pos };
	}

	float input::get_mouse_x_pos()
	{
		auto [x, y] = get_mouse_pos();
		return x;
	}

	float input::get_mouse_y_pos()
	{
		auto [x, y] = get_mouse_pos();
		return y;
	}
}