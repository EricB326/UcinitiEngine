#ifndef WINDOW_H
#define WINDOW_H

/* Pre-comp header include
*/
#include "uvkpch.h"

/* Uciniti engine includes
*/
#include "base.h"
#include "Uciniti/Events/event.h"

namespace Uciniti
{
	/* @brief */
	struct window_properties
	{
		std::string title;
		uint16_t width;
		uint16_t height;

		window_properties(const std::string& a_title = "Uciniti Engine",
						uint16_t a_width = 1280,
						uint16_t a_height = 720)
			: title(a_title), width(a_width), height(a_height)
		{}
	};

	/* @brief Interface representing a desktop system based window.
	*/
	class UCINITI_API window
	{
	public:
		using event_callback_fn = std::function<void(event&)>;

		virtual ~window() {}

		virtual void on_update() = 0;

		virtual uint16_t get_width() const = 0;

		virtual uint16_t get_height() const = 0;

		virtual void set_event_callback(const event_callback_fn& a_callback) = 0;

		virtual void set_vsync(bool a_enabled) = 0;

		virtual bool is_vsync() const = 0;

		/* @brief As this interface is independent, this create function has to be
				  implemented per platform.
		*/
		static window* create(const window_properties& a_props = window_properties());
	};
}

#endif // !WINDOW_H