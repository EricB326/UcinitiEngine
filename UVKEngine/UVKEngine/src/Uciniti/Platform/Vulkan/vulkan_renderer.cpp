#include "uvkpch.h"
#include "vulkan_renderer.h"

#include "Uciniti/Platform/Vulkan/vulkan_context.h"

#include "Uciniti/Renderer/renderer.h"

#include "Uciniti/Platform/Vulkan/vulkan_framebuffer.h"
#include "Uciniti/Platform/Vulkan/vulkan_pipeline.h"
#include "Uciniti/Platform/Vulkan/vulkan_render_pass.h"

namespace Uciniti
{
	//static ref_ptr<framebuffer> s_framebuffer;
	static pipeline* mesh_pipeline;

	void vulkan_renderer::init()
	{
		// #TODO: Framebuffer and render passes aren't setup here properly yet. Currently just in the swap chain.
		// Mesh pipeline setup.
		framebuffer_spec spec;
		//s_framebuffer = framebuffer::create(spec);

		pipeline_spec pipeline_specification;
		pipeline_specification._shader = renderer::get_shader_library()->get("vulkan_triangle");

		render_pass_spec render_pass_specification;
		//render_pass_specification.target_framebuffer = s_framebuffer;
		pipeline_specification._render_pass = render_pass::create(render_pass_specification);

		mesh_pipeline = pipeline::create(pipeline_specification);
	}

	void vulkan_renderer::draw()
	{
		//renderer::submit([=]()
		{
			ref_ptr<vulkan_context> context = vulkan_context::get();
			vulkan_pipeline* draw_pipeline = (vulkan_pipeline*)mesh_pipeline;

			vulkan_swap_chain& swap_chain = context->get_swap_chain();

			VkCommandBufferBeginInfo cmd_buf_info(vk_base_command_buffer_begin_info);

			VkClearValue clear_values;
			clear_values.color = { {0.1f, 0.1f, 0.1f, 1.0f} };

			VkRenderPassBeginInfo render_pass_begin_info(vk_base_render_pass_being_info);
			render_pass_begin_info.renderPass = swap_chain.get_render_pass();
			render_pass_begin_info.renderArea.offset.x = 0;
			render_pass_begin_info.renderArea.offset.y = 0;
			render_pass_begin_info.renderArea.extent.width = swap_chain.get_width();
			render_pass_begin_info.renderArea.extent.height = swap_chain.get_height();
			render_pass_begin_info.clearValueCount = 1; // Colour.
			render_pass_begin_info.pClearValues = &clear_values;

			for (size_t i = 0; i < swap_chain.get_image_count(); i++)
			{
				render_pass_begin_info.framebuffer = swap_chain.get_framebuffer(i);
				VkCommandBuffer draw_cmd_buf = swap_chain.get_draw_command_buffer(i);

				VK_CHECK_RESULT(vkBeginCommandBuffer(draw_cmd_buf, &cmd_buf_info));

					vkCmdBeginRenderPass(draw_cmd_buf, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

						// Update dynamic viewport and scissor states.
						VkViewport viewport = {};
						viewport.x = 0.0f;
						viewport.y = 0.0f;
						viewport.width = swap_chain.get_width();
						viewport.height = swap_chain.get_height();
						viewport.minDepth = 0.0f;
						viewport.maxDepth = 1.0f;
						vkCmdSetViewport(draw_cmd_buf, 0, 1, &viewport);

						VkRect2D scissor = {};
						scissor.extent.width = swap_chain.get_width();
						scissor.extent.height = swap_chain.get_height();
						scissor.offset = { 0, 0 };
						vkCmdSetScissor(draw_cmd_buf, 0, 1, &scissor);
						
						// #TODO: In the future this should instead be a for each loop going over each mesh and rendering them.

						vkCmdBindPipeline(draw_cmd_buf, VK_PIPELINE_BIND_POINT_GRAPHICS, draw_pipeline->get_graphics_pipeline());
					
						vkCmdDraw(draw_cmd_buf, 3, 1, 0, 0);

					vkCmdEndRenderPass(draw_cmd_buf);

				VK_CHECK_RESULT(vkEndCommandBuffer(draw_cmd_buf));
			}
			

		}//);
	}

	void vulkan_renderer::shutdown()
	{
		delete mesh_pipeline;
		mesh_pipeline = nullptr;
	}

	void vulkan_renderer::composite_render_pass(VkCommandBufferInheritanceInfo& a_inheritance_info)
	{

	}

}