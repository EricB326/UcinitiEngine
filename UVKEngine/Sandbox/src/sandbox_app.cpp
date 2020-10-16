/* Uciniti engine includes
*/
#include <uciniti.h>

class sandbox_layer : public Uciniti::layer
{
public:
	sandbox_layer()
	{
	}

	virtual ~sandbox_layer()
	{
	}

	virtual void on_attach() override
	{
	}

	virtual void on_detach() override
	{
	}

	virtual void on_update() override
	{
		//UVK_INFO("example_layer::update");
		//if (Uciniti::input::is_key_pressed(Uciniti::key_code::tab))
		//	UVK_TRACE("Tab key is pressed!");

		//if (Uciniti::input::is_mouse_button_pressed(UVK_MOUSE_BUTTON_LEFT))
		//	UVK_TRACE("Left click!");
	}

	virtual void on_event(Uciniti::event& a_event) override
	{
		//if (a_event.get_event_type() == Uciniti::event_type::key_pressed)
		//{
		//	Uciniti::key_pressed_event& e = (Uciniti::key_pressed_event&)a_event;
		//	UVK_TRACE("{0}", (char)e.get_key_code());
		//}
	}
};

/* @brief
*  @inherit
*/
class sandbox_app : public Uciniti::application
{
public:
	/***************************************************************/
	// Public Functions
	/***************************************************************/
	/* @brief
	*/
	sandbox_app()
	{
		push_layer(new sandbox_layer());
	}

	/* @brief
	*/
	~sandbox_app()
	{
	}
};

/* Entry point declaration
*/
Uciniti::application* Uciniti::create_application()
{
	return new sandbox_app();
}