#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include "Uciniti/Platform/Vulkan/vulkan_base.h"
#include "Uciniti/Core/time_step.h"

namespace Uciniti
{
	class vulkan_renderer
	{
	public:

		static void init();
		static void draw(time_step a_time_step);
		static void shutdown();

	private:
		void composite_render_pass(VkCommandBufferInheritanceInfo& a_inheritance_info);

	};
}

#endif // !VULKAN_RENDERER_H
