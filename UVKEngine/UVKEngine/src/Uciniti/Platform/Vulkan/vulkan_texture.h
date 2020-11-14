#ifndef VULKAN_TEXTURE_H
#define VULKAN_TEXTURE_H

#include "Uciniti/Renderer/texture.h"

#include "Uciniti/Platform/Vulkan/vulkan_base.h"

namespace Uciniti
{
	class vulkan_texture2D : public texture2D
	{
	public:
		vulkan_texture2D(const std::string& a_path);
		virtual ~vulkan_texture2D();

		void invalidate();

		virtual void bind(uint32_t a_slot = 0) const override;

		virtual uint32_t get_width() const override { return width; }
		virtual uint32_t get_height() const override { return height; }
	
	private:
		std::string filepath;
		uint32_t width, height;

		//Buffer image_data;

		VkDeviceMemory device_memory;
		VkImage image;

		VkDescriptorImageInfo descriptor_image_info = {};
	};
}

#endif // !VULKAN_TEXTURE_H