#ifndef APPLICATION_H
#define APPLICATION_H

#include "core.h"

namespace Uciniti 
{
	class UCINITI_API application
	{
	public:
		application();
		virtual ~application();

		void run();
	};

	application* create_application();
}

#endif // !APPLICATION_H