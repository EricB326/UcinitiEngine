#ifndef LAYER_H
#define LAYER_H

/* Uciniti engine includes
*/
#include "base.h"
#include "Uciniti/Core/Events/event.h"
#include "Uciniti/Core/time_step.h"

namespace Uciniti
{
	class UCINITI_API layer
	{
	public:
		layer(const std::string& a_name = "Layer");
		virtual ~layer();

		virtual void on_attach() {}
		virtual void on_detach() {}
		virtual void on_update(time_step a_time_step) {}
		virtual void on_event(event& a_event) {}

		inline const std::string& get_name() const { return debug_name; }

	protected:
		std::string debug_name;
	};
}

#endif // !LAYER_H`