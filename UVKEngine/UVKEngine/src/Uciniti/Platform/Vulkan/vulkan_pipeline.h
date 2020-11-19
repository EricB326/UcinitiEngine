#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H

#include "vulkan_base.h"

#include "Uciniti/Renderer/pipeline.h"

namespace Uciniti
{
	/* @brief Struct holding all of the different create infos for the graphics pipeline.
			  Each create info is set as default to a set of values via the constructor.
	*/
	struct pipeline_info
	{
		/* @brief Sets all create infos to base values using the create_info_base.h
		*/
		pipeline_info();

		VkPipelineVertexInputStateCreateInfo vertex_info;
		VkPipelineInputAssemblyStateCreateInfo assembly_info;
		VkPipelineTessellationStateCreateInfo tessellation_info;
		VkPipelineViewportStateCreateInfo viewport_info;
		VkPipelineRasterizationStateCreateInfo rasterinzation_info;
		VkPipelineMultisampleStateCreateInfo multisample_info;
		VkPipelineDepthStencilStateCreateInfo depth_stencil_info;
		VkPipelineColorBlendAttachmentState colour_blend_attachment;
		VkPipelineColorBlendStateCreateInfo colour_blend_info;
		VkPipelineDynamicStateCreateInfo dynamic_state_info;
		VkPipelineLayoutCreateInfo pipeline_layout_info;
	};

	class vulkan_pipeline : public pipeline
	{
	public:
		vulkan_pipeline(const pipeline_spec& a_spec);
		virtual ~vulkan_pipeline();

		virtual void init() override;
		void shutdown();

		const VkPipeline get_graphics_pipeline() const { return graphics_pipeline; }
		const VkPipelineLayout get_pipeline_layout() const { return pipeline_layout_handle; }

		virtual pipeline_spec& get_specification() override { return spec; }
		virtual const pipeline_spec& get_specification() const { return spec; }

	private:
		pipeline_spec spec;

		/* @brief Handle of the pipeline layout created.
		*/
		VkPipelineLayout pipeline_layout_handle;
		VkPipeline graphics_pipeline;

		pipeline_info pipeline_data;

		std::vector<VkDynamicState> dynamic_states;
		VkVertexInputBindingDescription vertex_input_binding;
		std::vector<VkVertexInputAttributeDescription> vertex_input_attributes;

		// #TODO: Remove this function. The pipeline should be dynamic and probably receive its data,
		//		  from the vulkan_renderer.cpp pre-draw command. Should be dynamic for the viewport,
		//		  and scissor states minimum. For now, temporary for the minimal viable product.
		void prepare_pipeline();

		void set_vertex_data();
	};
}

#endif // !VULKAN_PIPELINE_H