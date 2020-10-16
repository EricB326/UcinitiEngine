#ifndef INPUT_H
#define INPUT_H

#include "base.h"
#include "key_codes.h"

namespace Uciniti
{
	class UCINITI_API input
	{
	public:
		static bool is_key_pressed(key_code a_keycode);

		static bool is_mouse_button_pressed(int a_button);
		static std::pair<float, float> get_mouse_pos();
		static float get_mouse_x_pos();
		static float get_mouse_y_pos();

	//public:
	//	inline static bool is_key_pressed(key_code a_keycode) { return instance->is_key_pressed_impl(a_keycode); }
	//	
	//	inline static bool is_mouse_button_pressed(int a_button) { return instance->is_mouse_button_pressed_impl(a_button); }
	//	inline static std::pair<float, float> get_mouse_pos() { return instance->get_mouse_pos_impl(); }
	//	inline static float get_mouse_x_pos() { return instance->get_mouse_x_pos_impl(); }
	//	inline static float get_mouse_y_pos() { return instance->get_mouse_y_pos_impl(); }
	//
	//protected:
	//	/* What is implemented per platform by the interface.
	//	*/
	//	virtual bool is_key_pressed_impl(key_code a_keycode) = 0;
	//
	//	virtual bool is_mouse_button_pressed_impl(int a_button) = 0;
	//	virtual std::pair<float, float> get_mouse_pos_impl() = 0;
	//	virtual float get_mouse_x_pos_impl() = 0;
	//	virtual float get_mouse_y_pos_impl() = 0;
	//
	//private:
	//	static input* instance;
	};
}

#endif // !INPUT_H