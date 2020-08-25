#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

/* Standard includes
*/
#include <sstream>

/* Uciniti engine includes
*/
#include "event.h"

namespace Uciniti
{
	class UCINITI_API mouse_moved_event : public event
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief Construct the mouse move event with the X and Y of the mouse as
				  it currently stands.
		*/
		mouse_moved_event(float a_x, float a_y)
			: mouse_x(a_x), mouse_y(a_y)
		{}

		/* @brief Retrieve the X value of the mouse.
		*/
		inline float get_x() const { return mouse_x; }

		/* @brief Retrieve the Y value of the mouse.
		*/
		inline float get_y() const { return mouse_y; }

		/* @brief Override the base ToString() to now display that a mouse moved
		*		  event has taken place the  values in the mouse X and Y.
		*/
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "mouse_moved_event: " << mouse_x << ", " << mouse_y;
			return ss.str();
		}

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_TYPE(mouse_moved);

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_CATEGORY(event_category_mouse | event_category_input)

	private:
		/***************************************************************/
		// Private Varaibles
		/***************************************************************/
		/* @brief The mouse X and Y values.S
		*/
		float mouse_x, mouse_y;
	};

	class UCINITI_API mouse_scrolled_event : public event
	{
	public:
		mouse_scrolled_event(float a_x_offset, float a_y_offset)
			: x_offset(a_x_offset), y_offset(a_y_offset)
		{}

		/* @brief Retrieve the X offset of the mouse.
		*/
		inline float get_x_offset() const { return x_offset; }

		/* @brief Retrieve the Y offset of the mouse.
		*/
		inline float get_y_offset() const { return y_offset; }

		/* @brief Override the base ToString() to now display that a mouse scrolled
		*		  event has taken place the values in the offset X and Y.
		*/
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "mouse_scrolled_event: " << get_x_offset() << ", " << get_y_offset();
			return ss.str();
		}

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_TYPE(mouse_scrolled);

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_CATEGORY(event_category_mouse | event_category_input)

	private:
		/***************************************************************/
		// Private Varaibles
		/***************************************************************/
		/* @brief The offset of the X and Y scroll.
		*/
		float x_offset, y_offset;
	};
}

#endif // !MOUSE_EVENT_H