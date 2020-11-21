#include "uvkpch.h"
#include "texture.h"

#include "Uciniti/Renderer/renderer_api.h"
#include "Uciniti/Platform/Vulkan/vulkan_image.h"

namespace Uciniti
{

	ref_ptr<texture2D> texture2D::create(const std::string& a_path)
	{
		switch (renderer_api::get_current_api())
		{
		case renderer_api_type::none: return nullptr;
		case renderer_api_type::vulkan: return create_ref_ptr<vulkan_texture2D>(a_path);
		}

		UVK_CORE_ASSERT(false, "Unknown Rendering API type.");
		return nullptr;
	}

}