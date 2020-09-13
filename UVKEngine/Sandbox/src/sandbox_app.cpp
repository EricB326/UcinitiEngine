/* Uciniti engine includes
*/
#include <uciniti.h>

class example_layer : public Uciniti::layer
{
public:
	example_layer()
		: layer("Example")
	{
	}

	void on_update() override
	{
		UVK_INFO("example_layer::update");
	}
	
	void on_event(Uciniti::event& a_event) override
	{
		UVK_INFO("{0}", a_event);
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
		push_layer(new example_layer());
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