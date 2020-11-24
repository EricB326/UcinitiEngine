#include "uvkpch.h"
#include "renderer.h"

#include "renderer_api.h"

#include "Uciniti/Renderer/shader.h"

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
		
		renderer::get_shader_library()->load("assets/shaders/vulkan_triangle.glsl");
		renderer::get_shader_library()->load("assets/shaders/static_shader.glsl");
		//renderer::get_shader_library()->load("assets/shaders/texture.glsl");
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