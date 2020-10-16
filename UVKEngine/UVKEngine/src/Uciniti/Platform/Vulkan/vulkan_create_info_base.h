#ifndef VULKAN_CREATE_INFO_BASE_H
#define VULKAN_CREATE_INFO_BASE_H

#include "Vulkan/vulkan_core.h"

/**********************************************************************************/
/* The purpose of this header is mainly to avoid having to type out the 'sType'	  */
/* variable when creating create info structs. It is also used for common default */
/* data as in some Description structs.											  */
/**********************************************************************************/

constexpr static VkApplicationInfo vk_base_application_info
{
	VK_STRUCTURE_TYPE_APPLICATION_INFO,
	nullptr,
	"NO CURRENT APPLICATION NAME!",
	VK_MAKE_VERSION(0, 1, 0),
	"NO CURRENT ENGINE NAME!",
	VK_MAKE_VERSION(0, 1, 0),
	VK_API_VERSION_1_2
};

constexpr static VkInstanceCreateInfo vk_base_instance_info
{
	VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	nullptr,
	0,
	nullptr
};

constexpr static VkDebugUtilsMessengerCreateInfoEXT vk_base_debug_create_info
{
	VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
	nullptr,
	0,
	0,
	0,
	nullptr,
	nullptr
};

constexpr static VkDeviceQueueCreateInfo vk_base_device_queue_create_info
{
	VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
	nullptr,
	0,
	0,
	0,
	nullptr
};

constexpr static VkDeviceCreateInfo vk_base_device_create_info
{
	VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkSwapchainCreateInfoKHR vk_base_swap_chain_create_info
{
	VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
	nullptr,
	0
};

constexpr static VkImageViewCreateInfo vk_base_image_view_create_info
{
	VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkAttachmentDescription vk_base_attachment_desciprtion
{
	0,								  // Attachment desciption flags.
	VK_FORMAT_UNDEFINED,			  // Format to use for attachment.
	VK_SAMPLE_COUNT_1_BIT,			  // Number of samples to write for multisampling.
	VK_ATTACHMENT_LOAD_OP_CLEAR,	  // Describes what to do with attachment before rendering.
	VK_ATTACHMENT_STORE_OP_STORE,	  // Describes what to do with attachment after rendering.
	VK_ATTACHMENT_LOAD_OP_DONT_CARE,  // Describes what to do with stencil before rendering.
	VK_ATTACHMENT_STORE_OP_DONT_CARE, // Describes what to do with stencil after rendering.
	VK_IMAGE_LAYOUT_UNDEFINED,		  // Image data layout before render pass starts.
	VK_IMAGE_LAYOUT_UNDEFINED		  // Image data layout after render pass (to change to).
};

constexpr static VkSubpassDescription vk_base_subpass_description
{
	0,
	VK_PIPELINE_BIND_POINT_GRAPHICS
};

constexpr static VkRenderPassCreateInfo vk_base_render_pass_create_info
{
	VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkPipelineShaderStageCreateInfo vk_base_pipeline_shader_stage_create_info
{
	VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkPipelineVertexInputStateCreateInfo vk_base_pipeline_vertex_input_state_info
{
	VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
	nullptr,
	0,
	0,
	nullptr,
	0,
	nullptr
};

constexpr static VkPipelineInputAssemblyStateCreateInfo vk_base_pipeline_input_assembly_info
{
	VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
	nullptr,
	0,
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	VK_FALSE
};

constexpr static VkPipelineTessellationStateCreateInfo vk_base_pipeline_tessellation_state_info
{
	VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
	nullptr,
	0,
};

constexpr static VkPipelineViewportStateCreateInfo vk_base_pipeline_viewport_state_info
{
	VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkPipelineRasterizationStateCreateInfo vk_base_pipeline_rasterization_state_info
{
	VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
	nullptr,
	0,
	VK_FALSE,
	VK_FALSE,
	VK_POLYGON_MODE_FILL,
	VK_CULL_MODE_BACK_BIT,
	VK_FRONT_FACE_COUNTER_CLOCKWISE,
	VK_FALSE
};

constexpr static VkPipelineMultisampleStateCreateInfo vk_base_pipeline_multisample_state_info
{
	VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkPipelineDepthStencilStateCreateInfo vk_base_pipeline_depth_stencil_state_info
{
	VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkPipelineColorBlendAttachmentState vk_base_pipeline_colour_blend_attachment_state
{
	VK_TRUE,
	VK_BLEND_FACTOR_SRC_ALPHA,
	VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
	VK_BLEND_OP_ADD,
	VK_BLEND_FACTOR_ONE,
	VK_BLEND_FACTOR_ZERO,
	VK_BLEND_OP_ADD,
	VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
};

constexpr static VkPipelineColorBlendStateCreateInfo vk_base_pipeline_colour_blend_state_info
{
	VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkPipelineDynamicStateCreateInfo vk_base_pipeline_dynamic_state_info
{
	VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkPipelineLayoutCreateInfo vk_base_pipeline_layout_info
{
	VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
	nullptr,
	0
};


constexpr static VkGraphicsPipelineCreateInfo vk_base_graphics_pipeline_create_info
{
	VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkShaderModuleCreateInfo vk_base_shader_module_create_info
{
	VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkFramebufferCreateInfo vk_base_framebuffer_create_info
{
	VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkCommandPoolCreateInfo vk_base_command_pool_create_info
{
	VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkCommandBufferAllocateInfo vk_base_command_buffer_alloc_info
{
	VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
	nullptr
};

constexpr static VkCommandBufferBeginInfo vk_base_command_buffer_begin_info
{
	VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
	nullptr,
	0
};

constexpr static VkRenderPassBeginInfo vk_base_render_pass_being_info
{
	VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
	nullptr
};

constexpr static VkSemaphoreCreateInfo vk_base_semaphore_create_info
{
	VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkFenceCreateInfo vk_base_fence_create_info
{
	VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
	nullptr,
	VK_FENCE_CREATE_SIGNALED_BIT
};

constexpr static VkBufferCreateInfo vk_base_buffer_create_info
{
	VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkMemoryAllocateInfo vk_base_memory_alloc_info
{
	VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
	nullptr
};

constexpr static VkSubmitInfo vk_base_submit_info
{
	VK_STRUCTURE_TYPE_SUBMIT_INFO,
	nullptr
};

constexpr static VkDescriptorSetLayoutCreateInfo vk_base_descriptor_set_layout_create_info
{
	VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkDescriptorPoolCreateInfo vk_base_descriptor_pool_create_info
{
	VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
	nullptr,
	0
};

constexpr static VkDescriptorSetAllocateInfo vk_base_descriptor_set_alloc_info
{
	VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
	nullptr
};

constexpr static VkWriteDescriptorSet vk_base_write_descriptor_set
{
	VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
	nullptr
};

constexpr static VkImageCreateInfo vk_base_image_create_info
{
	VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
	nullptr
};

constexpr static VkImageMemoryBarrier vk_base_image_memory_barrier
{
	VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
	nullptr
};

constexpr static VkSamplerCreateInfo vk_base_sampler_create_info
{
	VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
	nullptr
};

#endif // !VULKAN_CREATE_INFO_BASE_H