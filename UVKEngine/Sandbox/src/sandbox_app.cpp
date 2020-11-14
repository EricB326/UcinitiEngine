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

	virtual void on_update(Uciniti::time_step a_time_step) override
	{
		UVK_INFO("Delta time: {0}s ({1}ms)", a_time_step.get_seconds(), a_time_step.get_milliseconds());

		if (Uciniti::input::is_key_pressed(Uciniti::key_code::tab))
			UVK_TRACE("Tab key is pressed!");
	}

	virtual void on_event(Uciniti::event& a_event) override
	{
	}

private:

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