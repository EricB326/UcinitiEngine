#ifndef EVENT_H
#define EVENT_H

/* Uciniti engine includes
*/
#include "Uciniti/core.h"

namespace Uciniti
{
	enum class event_type
	{
		none = 0,
		window_close, window_resize, window_focus, window_lost_focus, window_moved,
		app_tick, app_update, app_render,
		key_pressed, key_released,
		mouse_button_pressed, mouse_button_released, mouse_moved, mouse_scrolled
	};

	enum event_category
	{
		none = 0,
		event_category_application  = BIT(0),
		event_category_input        = BIT(1),
		event_category_keyboard     = BIT(2),
		event_category_mouse        = BIT(3),
		event_category_mouse_button = BIT(4)
	};

/* @brief Macros created to prevent needing to do the function implementation in every event application.
*  @function Retrives the static of the event type. Used to avoid having an instance of an event class.
*  @function None-instanced based virtual getted of the event type. Used for event base class for polymorphic cases.
*  @function Returns the name of the type, usually for debugging purposes.
*/
#define EVENT_CLASS_TYPE(type) static event_type get_static_type() { return event_type::##type; }\
								virtual event_type get_event_type() const override { return get_static_type(); }\
								virtual const char* get_name() const override { return #type; }

/* @brief Macros created to prevent needing to do the function implementation in every event application.
*/
#define EVENT_CLASS_CATEGORY(category) virtual int get_category_flags() const override { return category; }

	/* @brief
	*/
	class UCINITI_API event
	{
		/* @brief
		*/
		friend class event_dispatcher;

	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief Pure virtual function for getting the event type.
		*/
		virtual event_type get_event_type() const = 0;

		// STARTNOTE: Probably should be only debugging visable.
		/* @brief Pure virtual function for getting the name.
		*/
		virtual const char* get_name() const = 0;
		// !ENDNOTE

		/* @brief Pure virtual function for getting the category flags.
		*/
		virtual int get_category_flags() const = 0;

		/* @brief Returns the name of the event with the ability to override.
		*/
		virtual std::string to_string() const { return get_name(); }

		/* @brief Utility function to check if an event is within a given category.
		   @param Category to check against.
		   @return Zero if it is not in the category, anything other than zero means it is.
		*/
		inline bool is_in_category(event_category a_category)
		{
			return get_category_flags() & a_category;
		}

	protected:
		/***************************************************************/
		// Protected Variables
		/***************************************************************/
		/* @brief Used to check if an event has been handled or not.
		*/
		bool handled = false;
	};

	/* @brief The dispatcher is used as a way to run events based on their type.
	*/
	class event_dispatcher
	{
		/* @brief T can be any event type listed, this event function is created with an appropraite class ie,
		*		 a window class may have an OnWindowResize event thats function would be used for this.
		*/
		template<typename T>
		using event_fn = std::function<bool(T&)>;

	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief Constructs a dispatcher event instance with the reference of the event passed.
		*/
		event_dispatcher(event& a_event)
			: _event(a_event)
		{}

		/* @brief Dispatch the event any number of times with the given event function.
		*  @param Function event being passed in with any T event type.
		*  @bool True if the function event ran, false if not.
		*/
		template<typename T>
		bool dispatch(event_fn<T> a_func)
		{
			/* @brief If the event constucted event type matches the type of the
					  event function, than the function can be ran.
			*/
			if (_event.get_event_type() == T::get_static_type())
			{
				_event.handled = func(*(T*)&_event);
				returnt true;
			}

			return false;
		}

	private:
		/***************************************************************/
		// Private Variables
		/***************************************************************/
		/* @brief The event passed through as reference, stored to check against.
		*/
		event& _event;
	};

	/* @brief Exists for the log library. Used to have ease of access when calling
			  to_string() on any given event.
	*/
	inline std::ostream& operator<<(std::ostream& a_os, const event& a_e)
	{
		return a_os << a_e.to_string();
	}
}

#endif // !EVENT_H
