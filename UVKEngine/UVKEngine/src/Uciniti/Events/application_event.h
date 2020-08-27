#ifndef APPLICATION_EVENT_H
#define APPLICATION_EVENT_H

/* Uciniti engine includes
*/
#include "event.h"

namespace Uciniti 
{
	/* @brief
	*/
	class UCINITI_API window_resize_event : public event
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief
		*/
		window_resize_event(uint16_t a_width, uint16_t a_height)
			: width(a_width), height(a_height)
		{}

		/* @brief Retrieve the width of the window.
		*/
		inline uint16_t get_width() const { return width; }

		/* @brief Retrieve the height of the window.
		*/
		inline uint16_t get_height() const { return height; }

		/* @brief Override the base to_string() to now display that a window resize
		*		  event has taken place and display the values in the width and height.
		*/
		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "window_resize_event: " << width << ", " << height;
			return ss.str();
		}

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_TYPE(window_resize);

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_CATEGORY(event_category_application)

	private:
		/***************************************************************/
		// Private Variables
		/***************************************************************/
		/* @brief Width and height of the window.
		*/
		uint16_t width, height;
	};

	/* @brief
	*/
	class UCINITI_API window_close_event : public event
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief
		*/
		window_close_event()
		{}

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_TYPE(window_close);

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_CATEGORY(event_category_application)
	};

	/* @brief
	*/
	class UCINITI_API app_tick_event : public event
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief
		*/
		app_tick_event()
		{}

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_TYPE(app_tick);

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_CATEGORY(event_category_application)
	};

	/* @brief
	*/
	class UCINITI_API app_update_event : public event
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief
		*/
		app_update_event()
		{}

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_TYPE(app_update);

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_CATEGORY(event_category_application)
	};

	/* @brief
	*/
	class UCINITI_API app_render_event : public event
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief
		*/
		app_render_event()
		{}

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_TYPE(app_render);

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_CATEGORY(event_category_application)
	};
}

#endif // !APPLICATION_EVENT_H