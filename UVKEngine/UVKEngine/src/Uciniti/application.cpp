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
#include "Uciniti/Events/application_event.h"
#include "Uciniti/log.h"

namespace Uciniti
{
	application::application()
	{
		window_context = std::unique_ptr<window>(window::create());
	}

	application::~application()
	{
	}

	void application::run()
	{
		while (is_running)
		{
			window_context->on_update();
		}
	}
}