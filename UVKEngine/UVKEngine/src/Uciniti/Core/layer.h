#ifndef LAYER_H
#define LAYER_H

/* Uciniti engine includes
*/
#include "core.h"
#include "Uciniti/Events/event.h"

namespace Uciniti
{
	class UCINITI_API layer
	{
	public:
		layer(const std::string& a_name = "Layer");
		virtual ~layer();

		virtual void on_attach() {}
		virtual void on_detach() {}
		virtual void on_update() {}
		virtual void on_event(event& a_event) {}

		inline const std::string& get_name() const { return debug_name; }

	protected:
		std::string debug_name;
	};
}

#endif // !LAYER_H`