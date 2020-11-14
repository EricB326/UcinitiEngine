#include "uvkpch.h"
#include "render_pass.h"

#include "Uciniti/Platform/Vulkan/vulkan_render_pass.h"

namespace Uciniti
{
	ref_ptr<render_pass> render_pass::create(const render_pass_spec& a_spec)
	{
		switch (renderer_api::get_current_api())
		{
			case renderer_api_type::none: return nullptr;
			case renderer_api_type::vulkan: return create_ref_ptr<vulkan_render_pass>(a_spec);
		}

		UVK_CORE_ASSERT(false, "Unknown Rendering API type.");
		return nullptr;
	}

}