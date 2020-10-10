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
	}

	virtual void on_event(Uciniti::event& a_event) override
	{
		//UVK_INFO("{0}", a_event);
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