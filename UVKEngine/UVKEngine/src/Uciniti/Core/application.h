#ifndef APPLICATION_H
#define APPLICATION_H

/* Uciniti engine includes
*/
#include "core.h"
#include "window.h"
#include "layer_stack.h"
#include "Uciniti/Events/event.h"
#include "Uciniti/Events/application_event.h"

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

		void on_event(event& a_e);

		void push_layer(layer* a_layer);
		void push_overlay(layer* a_overlay);

	private:
		bool on_window_close(window_close_event& a_e);

		std::unique_ptr<window> window_context;
		bool is_running = true;
		layer_stack app_layer_stack;
	};

	/* @brief To be defined in CLIENT.
	*/
	application* create_application();
}

#endif // !APPLICATION_H