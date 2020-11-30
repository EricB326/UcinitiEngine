#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include "Uciniti/Core/time_step.h"
#include "Uciniti/Platform/Vulkan/vulkan_base.h"
#include "Uciniti/Platform/Vulkan/vulkan_pipeline.h"
#include "Uciniti/Platform/Vulkan/vulkan_mesh.h"

namespace Uciniti
{
	class vulkan_renderer
	{
	public:

		static void submit_pipeline(const ref_ptr<pipeline>& a_pipeline);
		static void submit_mesh(const ref_ptr<mesh>& a_mesh);

		static void init();
		// #TODO: Draw should probably be called from the renderer.h to call the correct API based on
		// current in use API.
		static void draw(time_step a_time_step);
		static void shutdown();

	private:
		void composite_render_pass(VkCommandBufferInheritanceInfo& a_inheritance_info);

	};
}

#endif // !VULKAN_RENDERER_H
