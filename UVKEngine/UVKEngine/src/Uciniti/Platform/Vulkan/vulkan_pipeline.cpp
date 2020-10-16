#include "uvkpch.h"
#include "vulkan_pipeline.h"

#include "vulkan_shader.h"

namespace Uciniti
{
	// =================================================================
	// Vulkan Pipeline Info
	// =================================================================
	pipeline_info::pipeline_info()
		: vertex_info(vk_base_pipeline_vertex_input_state_info), assembly_info(vk_base_pipeline_input_assembly_info), tessellation_info(vk_base_pipeline_tessellation_state_info),
		viewport_info(vk_base_pipeline_viewport_state_info), rasterinzation_info(vk_base_pipeline_rasterization_state_info), multisample_info(vk_base_pipeline_multisample_state_info),
		colour_blend_attachment(vk_base_pipeline_colour_blend_attachment_state), depth_stencil_info(vk_base_pipeline_depth_stencil_state_info), colour_blend_info(vk_base_pipeline_colour_blend_state_info),
		dynamic_state_info(vk_base_pipeline_dynamic_state_info), pipeline_layout_info(vk_base_pipeline_layout_info)
	{}

	// =================================================================
	// Vulkan Pipeline
	// =================================================================

	vulkan_pipeline::vulkan_pipeline()
	{

	}

	vulkan_pipeline::~vulkan_pipeline()
	{

	}

	void vulkan_pipeline::create_pipeline()
	{
		//vulkan_shader vulkan_triangle_vert, vulkan_triangle_frag;
		//vulkan_triangle_vert.read_shader_from_file("../../../../Sandbox/assets/shaders/vert.spv");
		//vulkan_triangle_frag.read_shader_from_file("../../../../Sandbox/assets/shaders/frag.spv");
	}

}