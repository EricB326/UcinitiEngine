#ifndef VULKAN_PIPELINE_H
#define VULKAN_PIPELINE_H

#include "vulkan_base.h"


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

	class vulkan_pipeline
	{
	public:
		vulkan_pipeline();
		~vulkan_pipeline();

		void create_pipeline();

	private:
	
	};
}

#endif // !VULKAN_PIPELINE_H