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

namespace Uciniti
{
	#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	application* application::instance = nullptr;

	application::application()
	{
		UVK_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		window_context = std::unique_ptr<window>(window::create());
		window_context->set_event_callback(BIND_EVENT_FN(application::on_event));
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
			for (layer* this_layer : app_layer_stack)
				this_layer->on_update();

			window_context->on_update();
		}
	}

	bool application::on_window_close(window_close_event& a_e)
	{
		is_running = false;
		return true;
	}
}