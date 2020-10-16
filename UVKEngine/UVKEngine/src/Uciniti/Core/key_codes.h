#ifndef KEY_CODES_H
#define KEY_CODES_H

namespace Uciniti	
{
	typedef enum class key_code : uint16_t
	{
		// From glfw3.h
		space = 32,
		apostrophe = 39, /* ' */
		comma = 44, /* , */
		minus = 45, /* - */
		period = 46, /* . */
		slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		semicolon = 59, /* ; */
		equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		left_bracket = 91,  /* [ */
		backslash = 92,  /* \ */
		right_bracket = 93,  /* ] */
		grave_accent = 96,  /* ` */

		world1 = 161, /* non-US #1 */
		world2 = 162, /* non-US #2 */

		/* Function keys */
		escape = 256,
		enter = 257,
		tab = 258,
		backspace = 259,
		insert = 260,
		del = 261,
		right = 262,
		left = 263,
		down = 264,
		up = 265,
		page_up = 266,
		page_down = 267,
		home = 268,
		end = 269,
		caps_lock = 280,
		scroll_lock = 281,
		num_lock = 282,
		print_screen = 283,
		pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KP_decimal = 330,
		KP_divide = 331,
		KP_multiply = 332,
		KP_subtract = 333,
		KP_add = 334,
		KP_enter = 335,
		KP_equal = 336,

		left_shift = 340,
		left_control = 341,
		left_alt = 342,
		left_super = 343,
		right_shift = 344,
		right_control = 345,
		right_alt = 346,
		right_super = 347,
		menu = 348
	} key;

	inline std::ostream& operator<<(std::ostream& a_os, key_code a_key_code)
	{
		a_os << static_cast<int32_t>(a_key_code);
		return a_os;
	}
}

// From glfw3.h
#define UVK_KEY_SPACE           ::Uciniti::key::space
#define UVK_KEY_APOSTROPHE      ::Uciniti::key::apostrophe    /* ' */
#define UVK_KEY_COMMA           ::Uciniti::key::comma         /* , */
#define UVK_KEY_MINUS           ::Uciniti::key::minus         /* - */
#define UVK_KEY_PERIOD          ::Uciniti::key::period        /* . */
#define UVK_KEY_SLASH           ::Uciniti::key::slash         /* / */
#define UVK_KEY_0               ::Uciniti::key::D0
#define UVK_KEY_1               ::Uciniti::key::D1
#define UVK_KEY_2               ::Uciniti::key::D2
#define UVK_KEY_3               ::Uciniti::key::D3
#define UVK_KEY_4               ::Uciniti::key::D4
#define UVK_KEY_5               ::Uciniti::key::D5
#define UVK_KEY_6               ::Uciniti::key::D6
#define UVK_KEY_7               ::Uciniti::key::D7
#define UVK_KEY_8               ::Uciniti::key::D8
#define UVK_KEY_9               ::Uciniti::key::D9
#define UVK_KEY_SEMICOLON       ::Uciniti::key::semicolon     /* ; */
#define UVK_KEY_EQUAL           ::Uciniti::key::equal         /* = */
#define UVK_KEY_A               ::Uciniti::key::A
#define UVK_KEY_B               ::Uciniti::key::B
#define UVK_KEY_C               ::Uciniti::key::C
#define UVK_KEY_D               ::Uciniti::key::D
#define UVK_KEY_E               ::Uciniti::key::E
#define UVK_KEY_F               ::Uciniti::key::F
#define UVK_KEY_G               ::Uciniti::key::G
#define UVK_KEY_H               ::Uciniti::key::H
#define UVK_KEY_I               ::Uciniti::key::I
#define UVK_KEY_J               ::Uciniti::key::J
#define UVK_KEY_K               ::Uciniti::key::K
#define UVK_KEY_L               ::Uciniti::key::L
#define UVK_KEY_M               ::Uciniti::key::M
#define UVK_KEY_N               ::Uciniti::key::N
#define UVK_KEY_O               ::Uciniti::key::O
#define UVK_KEY_P               ::Uciniti::key::P
#define UVK_KEY_Q               ::Uciniti::key::Q
#define UVK_KEY_R               ::Uciniti::key::R
#define UVK_KEY_S               ::Uciniti::key::S
#define UVK_KEY_T               ::Uciniti::key::T
#define UVK_KEY_U               ::Uciniti::key::U
#define UVK_KEY_V               ::Uciniti::key::V
#define UVK_KEY_W               ::Uciniti::key::W
#define UVK_KEY_X               ::Uciniti::key::X
#define UVK_KEY_Y               ::Uciniti::key::Y
#define UVK_KEY_Z               ::Uciniti::key::Z
#define UVK_KEY_LEFT_BRACKET    ::Uciniti::key::left_bracket   /* [ */
#define UVK_KEY_BACKSLASH       ::Uciniti::key::Backslash     /* \ */
#define UVK_KEY_RIGHT_BRACKET   ::Uciniti::key::right_bracket  /* ] */
#define UVK_KEY_GRAVE_ACCENT    ::Uciniti::key::grave_accent   /* ` */
#define UVK_KEY_WORLD_1         ::Uciniti::key::world1        /* non-US #1 */
#define UVK_KEY_WORLD_2         ::Uciniti::key::world2        /* non-US #2 */
								
/* Function keys */				
#define UVK_KEY_ESCAPE          ::Uciniti::key::escape
#define UVK_KEY_ENTER           ::Uciniti::key::enter
#define UVK_KEY_TAB             ::Uciniti::key::tab
#define UVK_KEY_BACKSPACE       ::Uciniti::key::backspace
#define UVK_KEY_INSERT          ::Uciniti::key::insert
#define UVK_KEY_DELETE          ::Uciniti::key::del
#define UVK_KEY_RIGHT           ::Uciniti::key::right
#define UVK_KEY_LEFT            ::Uciniti::key::left
#define UVK_KEY_DOWN            ::Uciniti::key::down
#define UVK_KEY_UP              ::Uciniti::key::up
#define UVK_KEY_PAGE_UP         ::Uciniti::key::page_up
#define UVK_KEY_PAGE_DOWN       ::Uciniti::key::page_down
#define UVK_KEY_HOME            ::Uciniti::key::home
#define UVK_KEY_END             ::Uciniti::key::end
#define UVK_KEY_CAPS_LOCK       ::Uciniti::key::caps_lock
#define UVK_KEY_SCROLL_LOCK     ::Uciniti::key::scroll_lock
#define UVK_KEY_NUM_LOCK        ::Uciniti::key::num_lock
#define UVK_KEY_PRINT_SCREEN    ::Uciniti::key::print_screen
#define UVK_KEY_PAUSE           ::Uciniti::key::pause
#define UVK_KEY_F1              ::Uciniti::key::F1
#define UVK_KEY_F2              ::Uciniti::key::F2
#define UVK_KEY_F3              ::Uciniti::key::F3
#define UVK_KEY_F4              ::Uciniti::key::F4
#define UVK_KEY_F5              ::Uciniti::key::F5
#define UVK_KEY_F6              ::Uciniti::key::F6
#define UVK_KEY_F7              ::Uciniti::key::F7
#define UVK_KEY_F8              ::Uciniti::key::F8
#define UVK_KEY_F9              ::Uciniti::key::F9
#define UVK_KEY_F10             ::Uciniti::key::F10
#define UVK_KEY_F11             ::Uciniti::key::F11
#define UVK_KEY_F12             ::Uciniti::key::F12
#define UVK_KEY_F13             ::Uciniti::key::F13
#define UVK_KEY_F14             ::Uciniti::key::F14
#define UVK_KEY_F15             ::Uciniti::key::F15
#define UVK_KEY_F16             ::Uciniti::key::F16
#define UVK_KEY_F17             ::Uciniti::key::F17
#define UVK_KEY_F18             ::Uciniti::key::F18
#define UVK_KEY_F19             ::Uciniti::key::F19
#define UVK_KEY_F20             ::Uciniti::key::F20
#define UVK_KEY_F21             ::Uciniti::key::F21
#define UVK_KEY_F22             ::Uciniti::key::F22
#define UVK_KEY_F23             ::Uciniti::key::F23
#define UVK_KEY_F24             ::Uciniti::key::F24
#define UVK_KEY_F25             ::Uciniti::key::F25
								
/* Keypad */					
#define UVK_KEY_KP_0            ::Uciniti::key::KP0
#define UVK_KEY_KP_1            ::Uciniti::key::KP1
#define UVK_KEY_KP_2            ::Uciniti::key::KP2
#define UVK_KEY_KP_3            ::Uciniti::key::KP3
#define UVK_KEY_KP_4            ::Uciniti::key::KP4
#define UVK_KEY_KP_5            ::Uciniti::key::KP5
#define UVK_KEY_KP_6            ::Uciniti::key::KP6
#define UVK_KEY_KP_7            ::Uciniti::key::KP7
#define UVK_KEY_KP_8            ::Uciniti::key::KP8
#define UVK_KEY_KP_9            ::Uciniti::key::KP9
#define UVK_KEY_KP_DECIMAL      ::Uciniti::key::KP_decimal
#define UVK_KEY_KP_DIVIDE       ::Uciniti::key::KP_divide
#define UVK_KEY_KP_MULTIPLY     ::Uciniti::key::KP_multiply
#define UVK_KEY_KP_SUBTRACT     ::Uciniti::key::KP_subtract
#define UVK_KEY_KP_ADD          ::Uciniti::key::KP_add
#define UVK_KEY_KP_ENTER        ::Uciniti::key::KP_enter
#define UVK_KEY_KP_EQUAL        ::Uciniti::key::KP_equal
								
#define UVK_KEY_LEFT_SHIFT      ::Uciniti::key::left_shift
#define UVK_KEY_LEFT_CONTROL    ::Uciniti::key::left_control
#define UVK_KEY_LEFT_ALT        ::Uciniti::key::left_alt
#define UVK_KEY_LEFT_SUPER      ::Uciniti::key::left_super
#define UVK_KEY_RIGHT_SHIFT     ::Uciniti::key::right_shift
#define UVK_KEY_RIGHT_CONTROL   ::Uciniti::key::right_control
#define UVK_KEY_RIGHT_ALT       ::Uciniti::key::right_alt
#define UVK_KEY_RIGHT_SUPER     ::Uciniti::key::right_super
#define UVK_KEY_MENU            ::Uciniti::key::menu

// Mouse
#define UVK_MOUSE_BUTTON_LEFT    0
#define UVK_MOUSE_BUTTON_RIGHT   1
#define UVK_MOUSE_BUTTON_MIDDLE  2

#endif // !KEY_CODES_H