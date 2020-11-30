#include "uvkpch.h"
#include "pipeline.h"

#include "Uciniti/Renderer/renderer_api.h"

#include "Uciniti/Platform/Vulkan/vulkan_pipeline.h"

namespace Uciniti
{
	ref_ptr<pipeline> pipeline::create(const pipeline_spec& a_spec)
	{
		switch (renderer_api::get_current_api())
		{
			case renderer_api_type::none: return nullptr;
			case renderer_api_type::vulkan: return create_ref_ptr<vulkan_pipeline>(a_spec);
		}

		UVK_CORE_ASSERT(false, "Unknown Rendering API type.");
		return nullptr;
	}
}