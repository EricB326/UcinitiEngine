#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#ifdef UVK_PLATFORM_WINDOWS

extern Uciniti::application* Uciniti::create_application();

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