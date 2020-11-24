#include "uvkpch.h"
#include "vulkan_renderer.h"

#include "Uciniti/Platform/Vulkan/vulkan_context.h"

#include "Uciniti/Renderer/renderer.h"

#include "Uciniti/Platform/Vulkan/vulkan_framebuffer.h"
#include "Uciniti/Platform/Vulkan/vulkan_pipeline.h"
#include "Uciniti/Platform/Vulkan/vulkan_render_pass.h"
#include "Uciniti/Platform/Vulkan/vulkan_buffer.h"
#include "Uciniti/Platform/Vulkan/vulkan_image.h"
#include "Uciniti/Platform/Vulkan/vulkan_mesh.h"


#include <chrono>

#include <glm/gtc/matrix_transform.hpp>

namespace Uciniti
{
	//static ref_ptr<framebuffer> s_framebuffer;
	static pipeline* mesh_pipeline;
	static vertex_buffer* quad_vertex_buffer;
	static index_buffer* quad_index_buffer;
	static VkDescriptorSet quad_descriptor_set;
	static std::vector<ref_ptr<mesh>> meshes;
	static ref_ptr<vulkan_shader> vk_shader;

	static void render_mesh(ref_ptr<mesh> a_mesh, VkCommandBuffer a_cmd_buffer)
	{
		vulkan_pipeline* draw_pipeline = (vulkan_pipeline*)mesh_pipeline;

		ref_ptr<vulkan_vertex_buffer> vk_vert_buffer = std::static_pointer_cast<vulkan_vertex_buffer>(a_mesh->get_vertex_buffer());
		VkBuffer vert_buffer = vk_vert_buffer->get_vertex_buffer();
		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(a_cmd_buffer, 0, 1, &vert_buffer, offsets);

		ref_ptr<vulkan_index_buffer> vk_index_buffer = std::static_pointer_cast<vulkan_index_buffer>(a_mesh->get_index_buffer());
		VkBuffer index_buffer = vk_index_buffer->get_index_buffer();
		vkCmdBindIndexBuffer(a_cmd_buffer, index_buffer, 0, VK_INDEX_TYPE_UINT32);

		std::vector<submesh> submeshes = a_mesh->get_submeshes();
		for (submesh& this_submesh : submeshes)
		{
			vkCmdBindPipeline(a_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, draw_pipeline->get_graphics_pipeline());

			// Bind descriptor sets 
			VkDescriptorSet* descriptor_set = std::static_pointer_cast<vulkan_mesh>(a_mesh)->get_descriptor_set();
			vkCmdBindDescriptorSets(a_cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, draw_pipeline->get_pipeline_layout(), 0, 1, descriptor_set, 0, nullptr);

			vkCmdDrawIndexed(a_cmd_buffer, this_submesh._index_count, 1, this_submesh._base_index, this_submesh._base_vertex, 0);
		}
	}

	void vulkan_renderer::init()
	{
		VkDevice device = vulkan_context::get()->get_logical_device()->get_logical_device();

		// #TODO: Framebuffer and render passes aren't setup here properly yet. Currently just in the swap chain.
		// Mesh pipeline setup.	
		//framebuffer_spec spec;
		//s_framebuffer = framebuffer::create(spec);

		pipeline_spec spec;
		spec._shader = renderer::get_shader_library()->get("static_shader");
		spec._shader->reload();
		spec._layout =
		{
			{ shader_data_type::_float3, "in_vert_position" },
			{ shader_data_type::_float3, "in_vert_normal" },
			{ shader_data_type::_float3, "in_vert_tangent" },
			{ shader_data_type::_float3, "in_vert_binormal" },
			{ shader_data_type::_float2, "in_vert_tex_coord" },
		};
		mesh_pipeline = pipeline::create(spec);

		vk_shader = std::static_pointer_cast<vulkan_shader>(spec._shader);

		meshes.push_back(mesh::create("assets/models/cerberus/Cerberus_LP.FBX"));

		//pipeline_spec pipeline_specification;
		//pipeline_specification._shader = renderer::get_shader_library()->get("vulkan_triangle");
		//
		//vk_shader = std::static_pointer_cast<vulkan_shader>(pipeline_specification._shader);
		//quad_texture = texture2D::create("assets/textures/texture.jpg");
		//vk_shader->set_sampler2d(1, std::static_pointer_cast<vulkan_texture2D>(quad_texture));
		//vk_shader->reload();
		//
		//pipeline_specification._layout =
		//{
		//	{ shader_data_type::_float3, "in_vert_position" },
		//	{ shader_data_type::_float3, "in_vert_colour" },
		//	{ shader_data_type::_float2, "in_vert_tex_coord" },
		//};
		//
		//render_pass_spec render_pass_specification;
		////render_pass_specification.target_framebuffer = s_framebuffer;
		//pipeline_specification._render_pass = render_pass::create(render_pass_specification);
		//
		//mesh_pipeline = pipeline::create(pipeline_specification);

		//struct quad_vertex
		//{
		//	glm::vec3 _pos;
		//	glm::vec3 _col;
		//	glm::vec2 _tex_coord;
		//};
		//
		//std::vector<quad_vertex> vertex_data =
		//{
		//	{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		//	{ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
		//	{ glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		//	{ glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
		//
		//	{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		//	{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
		//	{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
		//	{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) }
		//};
		//
		////quad_vertex_buffer = vertex_buffer::create(vertex_data.data(), 8 * sizeof(quad_vertex), vertex_data.size());
		//
		//std::vector<uint32_t> index_data =
		//{
		//	0, 1, 2, 2, 3, 0,
		//	4, 5, 6, 6, 7, 4
		//};
		//
		////quad_index_buffer = index_buffer::create(index_data.data(), 12 * sizeof(uint32_t));
		//
		//quad_descriptor_set = vk_shader->create_descriptor_set();
		//
		//ref_ptr<vulkan_texture2D> texture = std::static_pointer_cast<vulkan_texture2D>(quad_texture);
		//
		//std::vector<VkWriteDescriptorSet> descriptor_writes{};
		//descriptor_writes.push_back(*vk_shader->get_write_descriptor_set("in_vert_mvp"));
		//descriptor_writes.push_back(*vk_shader->get_write_descriptor_set("in_frag_tex_sampler"));
		//descriptor_writes[0].dstSet = quad_descriptor_set;
		//descriptor_writes[0].pBufferInfo = &vk_shader->get_buffer_info(0);
		//descriptor_writes[1].dstSet = quad_descriptor_set;
		//descriptor_writes[1].pImageInfo = &texture->get_descriptor();
		//vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptor_writes.size()), descriptor_writes.data(), 0, nullptr);
	}

	void vulkan_renderer::draw(time_step a_time_step)
	{
		//renderer::submit([=]()
		{
			ref_ptr<vulkan_context> context = vulkan_context::get();

			vulkan_swap_chain& swap_chain = context->get_swap_chain();

			VkCommandBufferBeginInfo cmd_buf_info(vk_base_command_buffer_begin_info);

			std::array<VkClearValue, 2> clear_values;
			clear_values[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
			clear_values[1].depthStencil = { 1.0f, 0 };

			VkRenderPassBeginInfo render_pass_begin_info(vk_base_render_pass_being_info);
			render_pass_begin_info.renderPass = swap_chain.get_render_pass();
			render_pass_begin_info.renderArea.offset.x = 0;
			render_pass_begin_info.renderArea.offset.y = 0;
			render_pass_begin_info.renderArea.extent.width = swap_chain.get_width();
			render_pass_begin_info.renderArea.extent.height = swap_chain.get_height();
			render_pass_begin_info.clearValueCount = static_cast<uint32_t>(clear_values.size());
			render_pass_begin_info.pClearValues = clear_values.data();

			render_pass_begin_info.framebuffer = swap_chain.get_framebuffer(swap_chain.get_current_image());
			VkCommandBuffer draw_cmd_buf = swap_chain.get_current_draw_command_buffer();

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
						
					for (ref_ptr<mesh>& this_mesh : meshes)
						render_mesh(this_mesh, draw_cmd_buf);

				vkCmdEndRenderPass(draw_cmd_buf);

			VK_CHECK_RESULT(vkEndCommandBuffer(draw_cmd_buf));
			
			static auto startTime = std::chrono::high_resolution_clock::now();
			auto currentTime = std::chrono::high_resolution_clock::now();
			float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
			
			// #TODO: In the future this should instead be a for each loop going over each mesh and rendering them.
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::rotate(model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
			glm::mat4 view = glm::lookAt(glm::vec3(2.0f, 0.3f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			glm::mat4 proj = glm::perspective(glm::radians(45.0f), swap_chain.get_width() / (float)swap_chain.get_height(), 0.1f, 1000.0f);
			proj[1][1] *= -1;
			vk_shader->set_member_matrix4(0, "model", model);
			vk_shader->set_member_matrix4(0, "view", view);
			vk_shader->set_member_matrix4(0, "proj", proj);
			
			void* data;
			vkMapMemory(context->get_logical_device()->get_logical_device(), vk_shader->get_uniform_buffer(0)._memory, 0, vk_shader->get_uniform_buffer(0)._size, 0, &data);
			
			std::vector<glm::mat4> data_to_copy;
			data_to_copy.push_back(vk_shader->get_uniform_buffer(0)._member_data._ubo_mat4["model"]);
			data_to_copy.push_back(vk_shader->get_uniform_buffer(0)._member_data._ubo_mat4["view"]);
			data_to_copy.push_back(vk_shader->get_uniform_buffer(0)._member_data._ubo_mat4["proj"]);
			memcpy(data, data_to_copy.data(), vk_shader->get_uniform_buffer(0)._size);
			
			vkUnmapMemory(context->get_logical_device()->get_logical_device(), vk_shader->get_uniform_buffer(0)._memory);
		}//);
	}

	void vulkan_renderer::shutdown()
	{
		delete mesh_pipeline;
		mesh_pipeline = nullptr;

		delete quad_vertex_buffer;
		quad_vertex_buffer = nullptr;

		delete quad_index_buffer;
		quad_index_buffer = nullptr;
	}

	void vulkan_renderer::composite_render_pass(VkCommandBufferInheritanceInfo& a_inheritance_info)
	{

	}

}