/* Pre-comp header include
*/
#include "uvkpch.h"

/* Class header include
*/
#include "application.h"

/* Vendor includes
*/
#include "GLFW/glfw3.h"

/* Uciniti engine includes
*/
#include "log.h"
#include "Uciniti/Renderer/renderer.h"
#include "Uciniti/Platform/Vulkan/vulkan_test_layer.h"
#include "Uciniti/Platform/Vulkan/vulkan_renderer.h"

namespace Uciniti
{
	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	application* application::instance = nullptr;

	application::application()
	{
		UVK_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		window_context = scope_ptr<window>(window::create());
		window_context->init_renderer();
		window_context->set_event_callback(BIND_EVENT_FN(application::on_event));

		renderer::init();
		vulkan_renderer::init();
		renderer::wait_and_render();

		//push_layer(new vulkan_test_layer());
	}

	application::~application()
	{
	}

	void application::push_layer(layer* a_layer)
	{
		app_layer_stack.push_layer(a_layer);
		a_layer->on_attach();
	}

	void application::push_overlay(layer* a_overlay)
	{
		app_layer_stack.push_overlay(a_overlay);
		a_overlay->on_attach();
		window::create();
	}

	void application::on_event(event& a_e)
	{
		event_dispatcher dispatcher(a_e);
		dispatcher.dispatch<window_resize_event>(BIND_EVENT_FN(application::on_window_resize));
		dispatcher.dispatch<window_close_event>(BIND_EVENT_FN(application::on_window_close));

		for (auto it = app_layer_stack.end(); it != app_layer_stack.begin();)
		{
			(*--it)->on_event(a_e);
			if (a_e.was_handled())
				break;
		}
	}

	void application::run()
	{
		while (is_running)
		{
			window_context->on_update();

			if (!_is_minimized)
			{
				for (layer* this_layer : app_layer_stack)
					this_layer->on_update(_time_step);

				window_context->get_render_context()->begin_frame();
				vulkan_renderer::draw(_time_step);
				window_context->swap_buffers();

			}

			// #TODO: Currently tied the class to glfw. Should be platform specific instead (linux::get_time()).
			float time = (float)glfwGetTime();
			_time_step = time - last_frame_time;
			last_frame_time = time;

		}

		shutdown();
	}



	bool application::on_window_resize(window_resize_event& a_e)
	{
		int width = a_e.get_width(), height = a_e.get_height();
		if (!width || !height)
		{
			_is_minimized = true;
			return false;
		}
		_is_minimized = false;

		return false;
	}

	bool application::on_window_close(window_close_event& a_e)
	{
		is_running = false;
		return true;
	}

	void application::shutdown()
	{
		vulkan_renderer::shutdown();
		//window_context->~window();
	}

}