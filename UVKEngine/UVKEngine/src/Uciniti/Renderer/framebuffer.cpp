#include "uvkpch.h"
#include "framebuffer.h"

#include "Uciniti/Platform/Vulkan/vulkan_framebuffer.h"

namespace Uciniti
{

	ref_ptr<framebuffer> framebuffer::create(const framebuffer_spec& a_spec)
	{
		switch (renderer_api::get_current_api())
		{
			case renderer_api_type::none: return nullptr;
			case renderer_api_type::vulkan: return create_ref_ptr<vulkan_framebuffer>(a_spec);
		}

		UVK_CORE_ASSERT(false, "Unknown Rendering API type.");
		return nullptr;
	}

}