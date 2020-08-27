#ifndef APPLICATION_H
#define APPLICATION_H

/* Uciniti engine includes
*/
#include "core.h"
#include "Events/event.h"

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
	};

	/* @brief To be defined in CLIENT.
	*/
	application* create_application();
}

#endif // !APPLICATION_H