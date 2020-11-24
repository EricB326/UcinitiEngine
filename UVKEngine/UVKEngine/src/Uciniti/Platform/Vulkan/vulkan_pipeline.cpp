#include "uvkpch.h"
#include "vulkan_pipeline.h"

#include "vulkan_shader.h"
#include "Uciniti/Platform/Vulkan/vulkan_context.h"
#include "Uciniti/Platform/Vulkan/vulkan_framebuffer.h"

namespace Uciniti
{
	static VkFormat shader_data_type_to_vulkan_format(shader_data_type a_type)
	{
		switch (a_type)
		{
		case Uciniti::shader_data_type::_float: return VK_FORMAT_R32_SFLOAT;
		case Uciniti::shader_data_type::_float2: return VK_FORMAT_R32G32_SFLOAT;
		case Uciniti::shader_data_type::_float3: return VK_FORMAT_R32G32B32_SFLOAT;
		case Uciniti::shader_data_type::_float4: return VK_FORMAT_R32G32B32A32_SFLOAT;
		}
	}

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

	vulkan_pipeline::vulkan_pipeline(const pipeline_spec& a_spec)
		: spec(a_spec)
	{
		init();
	}

	vulkan_pipeline::~vulkan_pipeline()
	{
		shutdown();
	}	

	void vulkan_pipeline::init()
	{
		VkDevice logical_device = vulkan_context::get()->get_logical_device()->get_logical_device();
		VkRenderPass render_pass = vulkan_context::get()->get_swap_chain().get_render_pass();

		ref_ptr<vulkan_shader> shader_to_use = ref_ptr<vulkan_shader>(std::static_pointer_cast<vulkan_shader>(spec._shader));
		//ref_ptr<vulkan_framebuffer> framebuffer = std::static_pointer_cast<vulkan_framebuffer>(spec._render_pass->get_specification().target_framebuffer);

		// Populate the pipeline data.
		prepare_pipeline();

		// Pipeline shader stage info.
		const std::vector<VkPipelineShaderStageCreateInfo>& shader_stages = shader_to_use->get_shader_stages();
		
		// Set pipeline data.
		VkGraphicsPipelineCreateInfo pipeline_create_info(vk_base_graphics_pipeline_create_info);
		pipeline_create_info.stageCount = static_cast<uint32_t>(shader_stages.size());
		pipeline_create_info.pStages = shader_stages.data();
		pipeline_create_info.pVertexInputState = &pipeline_data.vertex_info;
		pipeline_create_info.pInputAssemblyState = &pipeline_data.assembly_info;
		pipeline_create_info.pTessellationState = &pipeline_data.tessellation_info;
		pipeline_create_info.pViewportState = &pipeline_data.viewport_info;
		pipeline_create_info.pRasterizationState = &pipeline_data.rasterinzation_info;
		pipeline_create_info.pMultisampleState = &pipeline_data.multisample_info;
		pipeline_create_info.pDepthStencilState = &pipeline_data.depth_stencil_info;
		pipeline_create_info.pColorBlendState = &pipeline_data.colour_blend_info;
		pipeline_create_info.pDynamicState = &pipeline_data.dynamic_state_info;
		pipeline_create_info.layout = pipeline_layout_handle;			  // Pipeline layout pipeline should use.
		pipeline_create_info.renderPass = render_pass; // Render pass description the pipeline is compatible with.
		pipeline_create_info.subpass = 0;								  // Subpass of render pass to use with pipeline.
		pipeline_create_info.basePipelineHandle = VK_NULL_HANDLE;
		pipeline_create_info.basePipelineIndex = -1;

		// Create graphics pipeline.
		VK_CHECK_RESULT(vkCreateGraphicsPipelines(logical_device, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &graphics_pipeline));

		vkDestroyShaderModule(logical_device, shader_stages[0].module, nullptr);
		vkDestroyShaderModule(logical_device, shader_stages[1].module, nullptr);

	}

	void vulkan_pipeline::shutdown()
	{
		VkDevice logical_device = vulkan_context::get()->get_logical_device()->get_logical_device();

		vkDestroyPipeline(logical_device, graphics_pipeline, nullptr);
		vkDestroyPipelineLayout(logical_device, pipeline_layout_handle, nullptr);
	}

	void vulkan_pipeline::prepare_pipeline()
	{
		ref_ptr<vulkan_context> context = vulkan_context::get();
		ref_ptr<vulkan_shader> shader_to_use = ref_ptr<vulkan_shader>(std::static_pointer_cast<vulkan_shader>(spec._shader));
		VkDescriptorSetLayout descriptor_set_layout = shader_to_use->get_descriptor_set_layout();

		VkPipelineLayoutCreateInfo pipeline_layout_create_info(vk_base_pipeline_layout_info);
		pipeline_layout_create_info.setLayoutCount = 1;
		pipeline_layout_create_info.pSetLayouts = &descriptor_set_layout;
		VK_CHECK_RESULT(vkCreatePipelineLayout(context->get_logical_device()->get_logical_device(), &pipeline_layout_create_info, nullptr, &pipeline_layout_handle));
		
		// Populate viewport info.
		pipeline_data.viewport_info.viewportCount = 1;
		pipeline_data.viewport_info.scissorCount = 1;
		pipeline_data.viewport_info.pViewports = nullptr; // Set as dynamic states.
		pipeline_data.viewport_info.pScissors = nullptr;  // Set as dynamic states.

		// Populate dynamic states.
		dynamic_states.push_back(VK_DYNAMIC_STATE_VIEWPORT);
		dynamic_states.push_back(VK_DYNAMIC_STATE_SCISSOR);

		pipeline_data.dynamic_state_info.pDynamicStates = dynamic_states.data();
		pipeline_data.dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());

		// Populate rasterizer.
		// ---- SET IN THE INTI LIST AS THE BASE CREATE INFO ----
		// Polygon mode : Fill.
		// Cull mode    : Back bit.
		pipeline_data.rasterinzation_info.lineWidth = 1.0f;			  // How thick lines should be when drawn. -- GPU feature for larger.
		pipeline_data.rasterinzation_info.depthBiasEnable = VK_FALSE; // Whether to add depth bias to fragments (good for stopping "shadow acne" in shadow mapping).
		pipeline_data.rasterinzation_info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

		// Populate multi sampling.
		pipeline_data.multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		// #TODO: Populate depth stencil testing.
		pipeline_data.depth_stencil_info.depthTestEnable = VK_TRUE;		   // Enable checking depth to determin fragment write.
		pipeline_data.depth_stencil_info.depthWriteEnable = VK_TRUE;		   // Enable writing to depth buffer (to replace old values).
		pipeline_data.depth_stencil_info.depthCompareOp = VK_COMPARE_OP_LESS; // Comparison operation that allows an overwrite (is in front).
		pipeline_data.depth_stencil_info.depthBoundsTestEnable = VK_FALSE;    // Depth bounds test: does the depth value exist between two bounds.
		pipeline_data.depth_stencil_info.stencilTestEnable = VK_FALSE;		   // Should stencil test be enabled.

		// Populate colour blend.
		// ---- SET IN THE INTI LIST AS THE BASE CREATE INFO ----
		// Populate color blend attachment state (how blending is handled).
		// Base data already sufficient (see vk_base_pipeline_colour_blend_attachment_state in create_info_base.h)
		// Summarised colour blend factor: (VK_BLEND_FACTOR_SRC_ALPHA * new colour) VK_BLEND_OP_ADD (VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA * old colour).
		//								   (new colour alpha * new colour) + ((1 - new colour alpha) * old colour).
		// Summarised alpha blend factor:  (VK_BLEND_FACTOR_ONE * new alpha) VK_BLEND_OP_ADD (VK_BLEND_FACTOR_ZERO * old alpha).
		//								   (1 * new alpha) + (0 * old alpha) = new alpha.
		pipeline_data.colour_blend_info.logicOpEnable = VK_FALSE;
		pipeline_data.colour_blend_info.attachmentCount = 1;
		pipeline_data.colour_blend_info.pAttachments = &pipeline_data.colour_blend_attachment;

		set_vertex_data();
	}

	void vulkan_pipeline::set_vertex_data()
	{
 		buffer_layout& layout = spec._layout;

		vertex_input_binding = {};
		vertex_input_binding.binding = 0;
		vertex_input_binding.stride = layout.get_stride();
		vertex_input_binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		vertex_input_attributes.resize(layout.get_element_count());
		uint32_t location = 0;
		for (buffer_element this_element : layout)
		{
			vertex_input_attributes[location].binding = 0;
			vertex_input_attributes[location].location = location;
			vertex_input_attributes[location].format = shader_data_type_to_vulkan_format(this_element.type);
			vertex_input_attributes[location].offset = this_element.offset;
			location++;
		}

		pipeline_data.vertex_info.vertexBindingDescriptionCount = 1;
		pipeline_data.vertex_info.pVertexBindingDescriptions = &vertex_input_binding;
		pipeline_data.vertex_info.vertexAttributeDescriptionCount = vertex_input_attributes.size();
		pipeline_data.vertex_info.pVertexAttributeDescriptions = vertex_input_attributes.data();
	}

}