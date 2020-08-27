/* Pre-comp header include
*/
#include "uvkpch.h"

/* Class header include
*/
#include "application.h"

/* Uciniti engine includes
*/
#include "Uciniti/Events/application_event.h"
#include "Uciniti/log.h"

namespace Uciniti
{
	application::application()
	{
	}

	application::~application()
	{
	}

	void application::run()
	{
		window_resize_event e(1280, 720);

		if (e.is_in_category(event_category_application))
		{
			UVK_TRACE(e);
		}

		while (true);
	}
}