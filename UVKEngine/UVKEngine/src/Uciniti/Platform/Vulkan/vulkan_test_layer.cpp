#include "uvkpch.h"
#include "vulkan_test_layer.h"

namespace Uciniti
{

	vulkan_test_layer::vulkan_test_layer()
	{
	}

	vulkan_test_layer::~vulkan_test_layer()
	{
	}

	void vulkan_test_layer::on_attach()
	{
		framebuffer_spec spec;
	}

	void vulkan_test_layer::on_detach()
	{
	}

	void vulkan_test_layer::on_update(time_step a_time_step)
	{
		static glm::vec4 clear_colour = { 0.1f, 0.1f, 0.1f, 1.0f };
		render(clear_colour);
	}

	void vulkan_test_layer::on_event(event& a_event)
	{
	}

	void vulkan_test_layer::render(const glm::vec4& clearColor)
	{

	}

}