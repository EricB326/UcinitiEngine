#include "uvkpch.h"
#include "vulkan_texture.h"

#include "stb_image.h"

namespace Uciniti
{

	vulkan_texture2D::vulkan_texture2D(const std::string& a_path)
	{
		int temp_width, temp_height, temp_channels;
		stbi_set_flip_vertically_on_load(1);

		stbi_uc* image_data = stbi_load(a_path.c_str(), &temp_width, &temp_height, &temp_channels, 0);
		UVK_CORE_ASSERT(image_data, "Failed to load image!");

		width = temp_width;
		height = temp_height;
	}

	vulkan_texture2D::~vulkan_texture2D()
	{
		
	}

	void vulkan_texture2D::invalidate()
	{

	}

	void vulkan_texture2D::bind(uint32_t a_slot /*= 0*/) const
	{

	}

}