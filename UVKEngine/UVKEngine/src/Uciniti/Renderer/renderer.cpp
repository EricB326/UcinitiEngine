#include "uvkpch.h"
#include "renderer.h"

#include "renderer_api.h"

#include "Uciniti/Renderer/shader.h"
#include "Uciniti/Platform/Vulkan/vulkan_renderer.h"

namespace Uciniti
{
	// #TODO: Move and make a valid call (perhaps depending on OS).
	renderer_api_type renderer_api::current_renderer_api = renderer_api_type::vulkan;
	void renderer_api::set_api(renderer_api_type a_api)
	{
		current_renderer_api = a_api;
	}

	struct renderer_data
	{
		ref_ptr<render_pass> _active_render_pass;
		render_command _command_queue;
		ref_ptr<shader_library> _shader_library;
	};

	static renderer_data s_data;

	void renderer::init()
	{
		s_data._shader_library = ref_ptr<shader_library>(new shader_library());
	}

	void renderer::shutdown()
	{
		s_data._shader_library->shutdown();
	}

	void renderer::submit_pipeline(const ref_ptr<pipeline>& a_pipeline)
	{
		switch (renderer_api::get_current_api())
		{
		case renderer_api_type::none: return;
		case renderer_api_type::vulkan:
			vulkan_renderer::submit_pipeline(a_pipeline);
			return;
		}

		UVK_CORE_ASSERT(false, "Unknown Rendering API type.");
		return;
	}

	void renderer::submit_mesh(const ref_ptr<mesh>& a_mesh)
	{
		switch (renderer_api::get_current_api())
		{
			case renderer_api_type::none: return;
			case renderer_api_type::vulkan: 
				vulkan_renderer::submit_mesh(a_mesh);
				return;
		}

		UVK_CORE_ASSERT(false, "Unknown Rendering API type.");
		return;
	}

	ref_ptr<shader_library> renderer::get_shader_library()
	{
		return s_data._shader_library;
	}

	void renderer::wait_and_render()
	{
		s_data._command_queue.execute();
	}

	Uciniti::render_command& renderer::get_render_command_queue()
	{
		return s_data._command_queue;
	}

}