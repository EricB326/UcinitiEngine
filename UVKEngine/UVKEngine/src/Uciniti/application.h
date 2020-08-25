#ifndef APPLICATION_H
#define APPLICATION_H

/* Uciniti engine includes
*/
#include "core.h"

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

	/* @brief
	*/
	application* create_application();
}

#endif // !APPLICATION_H