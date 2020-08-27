#ifndef KEY_EVENT_H
#define KEY_EVENT_H

/* Uciniti engine includes
*/
#include "event.h"

namespace Uciniti
{
	/* @brief Contains common functionalty across key events (pressed/released/etc) such as
			  the key code.
	*/
	class UCINITI_API key_event : public event
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief Getter for retrieving the key code.
		   @return Int value of the key code.
		*/
		inline int get_key_code() const { return key_code; }

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_CATEGORY(event_category_keyboard | event_category_input)

	protected:
		/***************************************************************/
		// Protected Functions
		/***************************************************************/
		/* @brief Protected constructor as nothing else can construct a key_event.
		*		  Handles setting the key code when constucted.
		*/
		key_event(int a_key_code)
			: key_code(a_key_code) 
		{}
		
		/***************************************************************/
		// Protected Variables
		/***************************************************************/
		/* @brief Key code of the key being used.
		*/
		int key_code;
	};

	/* @brief Handles any key presses and if they have been repeated (held).
	*/
	class UCINITI_API key_pressed_event : public key_event
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief Construct the pressed event.
		*  @param Key code of the key being pressed.
		*  @param Repeat count, ie is this the first time the key was pressed,
		*		  or has it been held for n number of repeats.
		*/
		key_pressed_event(int a_key_code, int a_repeat_count)
			: key_event(a_key_code), repeat_count(a_repeat_count)
		{}

		/* @brief Get the repeat count of the key pressed.
		*/
		inline int get_repeat_count() const { return repeat_count; }

		/* @brief Override the base ToString() to now display which key code has been
		*		  pressed, as well as the repeat count.
		*/
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "key_pressed_event: " << key_code << " (" << repeat_count << " repeats)";
			return ss.str();
		}

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_TYPE(key_pressed)

	private:
		/***************************************************************/
		// Private Variables
		/***************************************************************/
		/* @brief The amount of times this key has been repeated.
		*/
		int repeat_count;
	};

	class UCINITI_API key_released_event : public key_event
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief Handles constructing the key event with the given key code that is being
				  released.
		*/
		key_released_event(int a_key_code)
			: key_event(a_key_code)
		{}

		/* @brief Override the base ToString() to now displace that a key release event
				  has taken place.
		*/
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "key_released_event: " << key_code;
			return ss.str();
		}

		/* @brief Ease of implementation for the functions specified in this macro.
		*		  See the event.h file.
		*/
		EVENT_CLASS_TYPE(key_released);
	};
}

#endif // !KEY_EVENT_H
