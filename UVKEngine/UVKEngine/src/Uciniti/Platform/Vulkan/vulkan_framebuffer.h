#ifndef VULKAN_FRAMEBUFFER_H
#define VULKAN_FRAMEBUFFER_H

#include "Uciniti/Renderer/framebuffer.h"
#include "Uciniti/Platform/Vulkan/vulkan_base.h"


namespace Uciniti 
{
	struct framebuffer_attachment
	{
		VkImage image;
		VkDeviceMemory mem;
		VkImageView view;
	};

	class vulkan_framebuffer : public framebuffer
	{
	public:
		vulkan_framebuffer(const framebuffer_spec& a_spec);
		virtual ~vulkan_framebuffer();

		// #TODO: This seems silly. Should probably just have some of init rather than
		//		  just "resize(...)" that doesn't receive any data as arguments. Setup a
		//		  "recreate(...)" / "resize(...)" or something similar that can take in that
		//		  data for framebuffer recreation.
		void resize(uint32_t a_width, uint32_t a_height, bool a_force_recreate);

		virtual const framebuffer_spec& get_specification() const override { return spec; }

		VkRenderPass get_render_pass() const { return _render_pass; }
		VkFramebuffer get_framebuffer() const { return vk_framebuffer; }


	private:
		framebuffer_spec spec;

		framebuffer_attachment colour_attachment;
		VkRenderPass _render_pass;
		VkFramebuffer vk_framebuffer;
		VkDescriptorImageInfo descriptor_image_info;
	};
}

#endif // !VULKAN_FRAMEBUFFER_H