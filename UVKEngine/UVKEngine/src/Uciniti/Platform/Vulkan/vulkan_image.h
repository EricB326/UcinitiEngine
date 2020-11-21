#ifndef VULKAN_TEXTURE_H
#define VULKAN_TEXTURE_H

#include "stb_image.h"

#include "Uciniti/Renderer/texture.h"

#include "vulkan_base.h"

namespace Uciniti
{
	// #TODO: Clean this up, take out data like stbi and sampler and move that into a texture specific data structure.
	struct image_spec
	{
		stbi_uc* _image_data;

		VkImage _image;
		VkDeviceMemory _memory;
		VkImageView _image_view;
		VkImageAspectFlags _aspect_flags;
		VkSampler _sampler;

		VkFormat _format;
		VkImageTiling _tiling;

		uint32_t _width, _height;
		uint32_t _size;
	};

	class vulkan_image_util
	{
	public:
		static void create_image(image_spec& a_spec, VkImageUsageFlags a_usage, VkMemoryPropertyFlags a_properties);
		static void create_image_view(image_spec& a_spec);
		static void transition_image(image_spec& a_spec, VkImageLayout a_old_layout, VkImageLayout a_new_layout);
		static void copy_buffer_to_image(image_spec& a_spec, VkBuffer& a_buffer);
	};

	class vulkan_texture2D : public texture2D
	{
	public:
		vulkan_texture2D() {}
		vulkan_texture2D(const std::string& a_path);
		virtual ~vulkan_texture2D();

		void init();
		void shutdown();

		virtual void bind(uint32_t a_slot = 0) const override;

		image_spec get_spec() const { return texture2d_image; }
		virtual uint32_t get_width() const override { return texture2d_image._width; }
		virtual uint32_t get_height() const override { return texture2d_image._height; }
		VkImageView get_image_view() const { return texture2d_image._image_view; }
		VkSampler get_sampler() const { return texture2d_image._sampler; }
		VkDescriptorImageInfo get_descriptor() const { return descriptor_image_info; }
	
	private:
		std::string filepath;

		image_spec texture2d_image;

		VkDescriptorImageInfo descriptor_image_info = {};

		void create_sampler();
	};
}

#endif // !VULKAN_TEXTURE_H