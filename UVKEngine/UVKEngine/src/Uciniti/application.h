#ifndef APPLICATION_H
#define APPLICATION_H

/* Uciniti engine includes
*/
#include "core.h"
#include "Events/event.h"
#include "window.h"

int main(int argc, char** argv);

namespace Uciniti 
{
	/* @brief
	*/
	class UCINITI_API application
	{
	public:
		/***************************************************************/
		// Public Functions
		/***************************************************************/
		/* @brief
		*/
		application();

		/* @brief
		*/
		virtual ~application();

		/* @brief
		*/
		void run();

	private:
		std::unique_ptr<window> window_context;
		bool is_running = true;
	};

	/* @brief To be defined in CLIENT.
	*/
	application* create_application();
}

#endif // !APPLICATION_H