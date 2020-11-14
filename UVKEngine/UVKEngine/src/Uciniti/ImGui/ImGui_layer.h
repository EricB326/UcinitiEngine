#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

/* Uciniti engine includes
*/
#include "Uciniti/Core/layer.h"
#include "Uciniti/Core/time_step.h"

namespace Uciniti
{
	class UCINITI_API ImGui_layer : public layer
	{
	public:
		ImGui_layer();
		~ImGui_layer();

		void on_attach() override;
		void on_detach() override;
		void on_update(time_step a_time_step) override;
		void on_event(event& a_event) override;

	private:
	};
}

#endif // !IMGUI_LAYER_H