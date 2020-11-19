#include "uvkpch.h"
#include "renderer_context.h"

#include "Uciniti/Renderer/renderer_api.h"


#include "Uciniti/Platform/Vulkan/vulkan_context.h"

namespace Uciniti
{
	ref_ptr<renderer_context> renderer_context::create(void* a_window)
	{
		switch (renderer_api::get_current_api())
		{
			case renderer_api_type::none: return nullptr;
			case renderer_api_type::vulkan: return create_ref_ptr<vulkan_context>(static_cast<GLFWwindow*>(a_window));
		}

		UVK_CORE_ASSERT(false, "Unknown Rendering API type.");
		return nullptr;
		
	}
}