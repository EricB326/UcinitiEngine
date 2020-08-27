#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

/* Uciniti engine includes
*/
#include "event.h"

namespace Uciniti
{
	/* @brief
	*/
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

		/* @brief Override the base to_string() to now display that a mouse moved
		*		  event has taken place the  values in the mouse X and Y.
		*/
		std::string to_string() const override
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

	/* @brief
	*/
	class UCINITI_API mouse_scrolled_event : public event
	{
	public:
		/* @brief
		*/
		mouse_scrolled_event(float a_x_offset, float a_y_offset)
			: x_offset(a_x_offset), y_offset(a_y_offset)
		{}

		/* @brief Retrieve the X offset of the mouse.
		*/
		inline float get_x_offset() const { return x_offset; }

		/* @brief Retrieve the Y offset of the mouse.
		*/
		inline float get_y_offset() const { return y_offset; }

		/* @brief Override the base to_string() to now display that a mouse scrolled
		*		  event has taken place the values in the offset X and Y.
		*/
		std::string to_string() const override
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

	/* @brief Base class to handle the button key pressed and released.
	*/
	class UCINITI_API mouse_button_event : public event
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief Retrieves the button pressed.
		*/
		inline int get_mouse_button() const { return button; }

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_CATEGORY(event_category_mouse | event_category_input)

	protected:
		/***************************************************************/
		// Protected Functions
		/***************************************************************/
		/* @brief Constructs to the button pressed being passed in.
		*/
		mouse_button_event(int a_button)
			: button(a_button)
		{}

		/***************************************************************/
		// Protected Variables
		/***************************************************************/
		/* @brief Stores the button that was pressed.
		*/
		int button;
	};

	/* @brief Handles mouse buttons that have be pressed.
	*/
	class UCINITI_API mouse_button_pressed_event : public mouse_button_event
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief Constructs a mouse button event with the button that was pressed.
		*/
		mouse_button_pressed_event(int a_button)
			: mouse_button_event(a_button)
		{}

		/* @brief Override the base to_string() to now display that a mouse button
		*		  pressed event has taken place the value of the button pressed.
		*/
		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "mouse_button_pressed_event: " << button;
			return ss.str();
		}

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_TYPE(mouse_button_pressed);
	};

	/* @brief Handles mouse buttons that have be released.
	*/
	class UCINITI_API mouse_button_released_event : public mouse_button_event
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief Constructs a mouse button event with the button that was released.
		*/
		mouse_button_released_event(int a_button)
			: mouse_button_event(a_button)
		{}

		/* @brief Override the base to_string() to now display that a mouse button
		*		  released event has taken place the value of the button released.
		*/
		std::string to_string() const override
		{
			std::stringstream ss;
			ss << "mouse_button_released_event: " << button;
			return ss.str();
		}

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_TYPE(mouse_button_released);
	};
}

#endif // !MOUSE_EVENT_H