#include <uciniti.h>

class sandbox_app : public Uciniti::application
{
public:
	sandbox_app()
	{
	}

	~sandbox_app()
	{
	}
};

Uciniti::application* Uciniti::create_application()
{
	return new sandbox_app();
}