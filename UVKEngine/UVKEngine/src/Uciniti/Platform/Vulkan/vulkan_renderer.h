#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include "Uciniti/Platform/Vulkan/vulkan_base.h"

namespace Uciniti
{
	class vulkan_renderer
	{
	public:

		static void init();
		static void draw();

	private:
		void composite_render_pass(VkCommandBufferInheritanceInfo& a_inheritance_info);

	};
}

#endif // !VULKAN_RENDERER_H
