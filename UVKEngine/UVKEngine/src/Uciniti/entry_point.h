#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

/* @brief If the platform is windows only, then the entry point can be created.
*/
#ifdef UVK_PLATFORM_WINDOWS

/* @brief Used to location an entry point delcaration made by the user for their
		  application.
*/
extern Uciniti::application* Uciniti::create_application();

/* @brief Create the entry point.
*/
int main(int argc, char** argv)
{
	auto app = Uciniti::create_application();

	app->run();

	delete app;
	app = nullptr;

	return 0;
}

#endif // UVK_PLATFORM_WINDOWS

#endif // !ENTRY_POINT_H