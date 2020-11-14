#ifndef VULKAN_TEST_LAYER_H
#define VULKAN_TEST_LAYER_H

#include "Uciniti/Core/layer.h"

#include "Uciniti/Renderer/pipeline.h"
#include "Uciniti/Renderer/framebuffer.h"

namespace Uciniti
{
	class vulkan_test_layer : public layer
	{
	public:
		vulkan_test_layer();
		virtual ~vulkan_test_layer();

		void on_attach() override;
		void on_detach() override;

		void on_update(time_step a_time_step) override;

		void on_event(event& a_event) override;

	private:
		ref_ptr<framebuffer> _framebuffer;

		void render(const glm::vec4& clearColor);
	};
}

#endif // !VULKAN_TEST_LAYER_H