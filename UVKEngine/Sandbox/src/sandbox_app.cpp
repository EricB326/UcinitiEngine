/* Uciniti engine includes
*/
#include <uciniti.h>
#include <Uciniti/entry_point.h>

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