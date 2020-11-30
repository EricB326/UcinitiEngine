#include "uvkpch.h"
#include "vulkan_framebuffer.h"

#include "Uciniti/Platform/Vulkan/vulkan_context.h"
#include "vulkan_create_info_base.h"
namespace Uciniti
{
	vulkan_framebuffer::vulkan_framebuffer(const framebuffer_spec& a_spec)
		: spec(a_spec)
	{
		resize(a_spec.width, a_spec.height, true);
	}

	vulkan_framebuffer::~vulkan_framebuffer()
	{

	}

	void vulkan_framebuffer::resize(uint32_t a_width, uint32_t a_height, bool a_force_recreate)
	{
		spec.width = a_width;
		spec.height = a_height;

		// If the framebuffer is not a swap chain target, than the framebuffer must be rendered off screen with
		// default settings. The settings are not pulled from the swapchain (ie, image, format, etc).
		if (!spec.swap_chain_target)
		{
			vulkan_framebuffer* instance = this;
			renderer::submit([instance, a_width, a_height]() mutable
			{
				ref_ptr<vulkan_logical_device> logical_device = vulkan_context::get()->get_logical_device();

				VkAttachmentDescription colour_attachment_desc(vk_base_attachment_description);

				// Colour attachment.
				{
					const VkFormat COLOUR_BUFFER_FORMAT = VK_FORMAT_R8G8B8A8_UNORM;

					// Create the image for the framebuffer.
					VkImageCreateInfo image_create_info(vk_base_image_create_info);
					image_create_info.imageType = VK_IMAGE_TYPE_2D;
					image_create_info.format = COLOUR_BUFFER_FORMAT;
					image_create_info.extent.width = a_width;
					image_create_info.extent.height = a_height;
					image_create_info.extent.depth = 1;
					image_create_info.mipLevels = 1;
					image_create_info.arrayLayers = 1;
					image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
					image_create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
					image_create_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

					VK_CHECK_RESULT(vkCreateImage(logical_device->get_logical_device(), &image_create_info, nullptr, &instance->colour_attachment.image));

					// Do some memory stuff here.
					VkMemoryRequirements mem_requires;
					vkGetImageMemoryRequirements(logical_device->get_logical_device(), instance->colour_attachment.image, &mem_requires);
					VkMemoryAllocateInfo mem_alloc(vk_base_memory_alloc_info);
					mem_alloc.allocationSize = mem_requires.size;
					mem_alloc.memoryTypeIndex = logical_device->get_physical_device_ref()->get_memory_type_index(mem_requires.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
					VK_CHECK_RESULT(vkAllocateMemory(logical_device->get_logical_device(), &mem_alloc, nullptr, &instance->colour_attachment.mem));
					VK_CHECK_RESULT(vkBindImageMemory(logical_device->get_logical_device(), instance->colour_attachment.image, instance->colour_attachment.mem, 0));

					// Create the image view.
					VkImageViewCreateInfo image_view_create_info(vk_base_image_view_create_info);
					image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
					image_view_create_info.format = COLOUR_BUFFER_FORMAT;
					image_view_create_info.subresourceRange = {};
					image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					image_view_create_info.subresourceRange.baseMipLevel = 0;
					image_view_create_info.subresourceRange.levelCount = 1;
					image_view_create_info.subresourceRange.baseArrayLayer = 0;
					image_view_create_info.subresourceRange.layerCount = 1;
					image_view_create_info.image = instance->colour_attachment.image;
					VK_CHECK_RESULT(vkCreateImageView(logical_device->get_logical_device(), &image_view_create_info, nullptr, &instance->colour_attachment.view));

					// Create sampler here.

					colour_attachment_desc.format = COLOUR_BUFFER_FORMAT;
					colour_attachment_desc.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				}

				VkAttachmentReference colour_reference = {};
				colour_reference.attachment = 0;
				colour_reference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				VkSubpassDescription subpass_description(vk_base_subpass_description);
				subpass_description.colorAttachmentCount = 1;
				subpass_description.pColorAttachments = &colour_reference;

				VkSubpassDependency dependency;
				dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				dependency.dstSubpass = 0;
				dependency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
				dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				dependency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				dependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

				// Create the render pass.
				VkRenderPassCreateInfo render_pass_info(vk_base_render_pass_create_info);
				render_pass_info.attachmentCount = 1;
				render_pass_info.pAttachments = &colour_attachment_desc;
				render_pass_info.subpassCount = 1;
				render_pass_info.pSubpasses = &subpass_description;
				VK_CHECK_RESULT(vkCreateRenderPass(logical_device->get_logical_device(), &render_pass_info, nullptr, &instance->_render_pass));

				// Doing this now but will be changed to an array for depth as well.
				VkImageView image_view_colour_attachment;
				image_view_colour_attachment = instance->colour_attachment.view;

				VkFramebufferCreateInfo framebuffer_create_info(vk_base_framebuffer_create_info);
				framebuffer_create_info.renderPass = instance->_render_pass;
				framebuffer_create_info.attachmentCount = 1;
				framebuffer_create_info.pAttachments = &image_view_colour_attachment;
				framebuffer_create_info.width = a_width;
				framebuffer_create_info.height = a_height;
				framebuffer_create_info.layers = 1;
				VK_CHECK_RESULT(vkCreateFramebuffer(logical_device->get_logical_device(), &framebuffer_create_info, nullptr, &instance->vk_framebuffer));

				// Fill a descriptor for later use.
				instance->descriptor_image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				instance->descriptor_image_info.imageView = instance->colour_attachment.view;
				// Do descriptor sampler once thats in.
			});
		}
		else
		{
			vulkan_swap_chain& swap_chain = vulkan_context::get()->get_swap_chain();
			_render_pass = swap_chain.get_render_pass();
		}
	}

}