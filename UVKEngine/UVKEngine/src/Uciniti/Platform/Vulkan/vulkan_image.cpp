#include "uvkpch.h"
#include "vulkan_image.h"

#include "vulkan_buffer.h"
#include "Uciniti/Platform/Vulkan/vulkan_context.h"

namespace Uciniti
{
	// =================================================================
	// Vulkan Image Utility
	// =================================================================
	void vulkan_image_util::create_image(image_spec& a_spec, VkImageUsageFlags a_usage, VkMemoryPropertyFlags a_properties)
	{
		ref_ptr<vulkan_logical_device> device = vulkan_context::get()->get_logical_device();

		VkImageCreateInfo image_info(vk_base_image_create_info);
		image_info.imageType = VK_IMAGE_TYPE_2D;
		image_info.extent.width = a_spec._width;
		image_info.extent.height = a_spec._height;
		image_info.extent.depth = 1;
		image_info.mipLevels = 1;
		image_info.arrayLayers = 1;
		image_info.format = a_spec._format;
		image_info.tiling = a_spec._tiling;
		image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		image_info.usage = a_usage;
		image_info.samples = VK_SAMPLE_COUNT_1_BIT;
		image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VK_CHECK_RESULT(vkCreateImage(device->get_logical_device(), &image_info, nullptr, &a_spec._image));

		VkMemoryRequirements mem_requirements;
		vkGetImageMemoryRequirements(device->get_logical_device(), a_spec._image, &mem_requirements);

		VkMemoryAllocateInfo alloc_info(vk_base_memory_alloc_info);
		alloc_info.allocationSize = mem_requirements.size;
		alloc_info.memoryTypeIndex = device->get_physical_device_ref()->get_memory_type_index(mem_requirements.memoryTypeBits, a_properties);
		VK_CHECK_RESULT(vkAllocateMemory(device->get_logical_device(), &alloc_info, nullptr, &a_spec._memory));

		VK_CHECK_RESULT(vkBindImageMemory(device->get_logical_device(), a_spec._image, a_spec._memory, 0));
	}

	void vulkan_image_util::create_image_view(image_spec& a_spec)
	{
		ref_ptr<vulkan_logical_device> device = vulkan_context::get()->get_logical_device();

		VkImageViewCreateInfo view_info(vk_base_image_view_create_info);
		view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view_info.image = a_spec._image;
		view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		view_info.format = a_spec._format;
		view_info.subresourceRange.aspectMask = a_spec._aspect_flags;
		view_info.subresourceRange.baseMipLevel = 0;
		view_info.subresourceRange.levelCount = 1;
		view_info.subresourceRange.baseArrayLayer = 0;
		view_info.subresourceRange.layerCount = 1;

		VK_CHECK_RESULT(vkCreateImageView(device->get_logical_device(), &view_info, nullptr, &a_spec._image_view));
	}

	void vulkan_image_util::transition_image(image_spec& a_spec, VkImageLayout a_old_layout, VkImageLayout a_new_layout)
	{
		VkCommandBuffer transition_command = vulkan_buffer_util::begin_one_time_command();

		VkImageMemoryBarrier barrier(vk_base_image_memory_barrier);
		barrier.oldLayout = a_old_layout;
		barrier.newLayout = a_new_layout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = a_spec._image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.srcAccessMask = 0; // #TODO: Fill in.
		barrier.dstAccessMask = 0; // #TODO: Fill in.	

		VkPipelineStageFlags source_stage;
		VkPipelineStageFlags dst_stage;

		if (a_old_layout == VK_IMAGE_LAYOUT_UNDEFINED && a_new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			source_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (a_old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && a_new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else 
			UVK_CORE_ASSERT(false, "Unsupported layout transision!");

		// Insert a memory dependency at the proper pipeline stages that will execute the image layout transition.
		// Source pipeline stage is host write/read execution (VK_PIPELINE_STAGE_HOST_BIT).
		// Destination pipeline stage is copy command execution (VK_PIPELINE_STAGE_TRANSFER_BIT).
		vkCmdPipelineBarrier
		(
			transition_command,
			source_stage,
			dst_stage,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);

		vulkan_buffer_util::end_one_time_command(transition_command);
	}

	void vulkan_image_util::copy_buffer_to_image(image_spec& a_spec, VkBuffer& a_buffer)
	{
		VkCommandBuffer copy_command = vulkan_buffer_util::begin_one_time_command();

		VkBufferImageCopy region;
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = { a_spec._width, a_spec._height, 1 };

		vkCmdCopyBufferToImage
		(
			copy_command,
			a_buffer,
			a_spec._image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

		vulkan_buffer_util::end_one_time_command(copy_command);
	}


	// =================================================================
	// Vulkan Texture2D
	// =================================================================
	vulkan_texture2D::vulkan_texture2D(const std::string& a_path)
	{
		int tex_width, tex_height, tex_channel;
		//stbi_set_flip_vertically_on_load(1);

		texture2d_image._image_data = stbi_load(a_path.c_str(), &tex_width, &tex_height, &tex_channel, STBI_rgb_alpha);
		UVK_CORE_ASSERT(texture2d_image._image_data, "Failed to load image!");

		texture2d_image._width = tex_width;
		texture2d_image._height = tex_height;
		texture2d_image._size = texture2d_image._width * texture2d_image._height * 4;

		init();
	}

	vulkan_texture2D::~vulkan_texture2D()
	{
		shutdown();
	}

	void vulkan_texture2D::init()
	{
		ref_ptr<vulkan_logical_device> device = vulkan_context::get()->get_logical_device();

		texture2d_image._format = VK_FORMAT_R8G8B8A8_SRGB;

		VkBuffer staging_buffer;
		VkDeviceMemory staging_buffer_mem;
		vulkan_buffer_util::create_buffer(vulkan_buffer_type::STAGING, staging_buffer, staging_buffer_mem, texture2d_image._size);

		void* data;
		vkMapMemory(device->get_logical_device(), staging_buffer_mem, 0, texture2d_image._size, 0, &data);
		memcpy(data, texture2d_image._image_data, static_cast<size_t>(texture2d_image._size));
		vkUnmapMemory(device->get_logical_device(), staging_buffer_mem);

		stbi_image_free(texture2d_image._image_data);

		texture2d_image._tiling = VK_IMAGE_TILING_OPTIMAL;
		vulkan_image_util::create_image(texture2d_image, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);;

		vulkan_image_util::transition_image(texture2d_image, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		vulkan_image_util::copy_buffer_to_image(texture2d_image, staging_buffer);
		vulkan_image_util::transition_image(texture2d_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	
		vkDestroyBuffer(device->get_logical_device(), staging_buffer, nullptr);
		vkFreeMemory(device->get_logical_device(), staging_buffer_mem, nullptr);

		texture2d_image._aspect_flags = VK_IMAGE_ASPECT_COLOR_BIT;
		vulkan_image_util::create_image_view(texture2d_image);
		create_sampler();

		descriptor_image_info.sampler = texture2d_image._sampler;
		descriptor_image_info.imageView = texture2d_image._image_view;
		descriptor_image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

	void vulkan_texture2D::bind(uint32_t a_slot /*= 0*/) const
	{
	}


	void vulkan_texture2D::create_sampler()
	{
		ref_ptr<vulkan_logical_device> device = vulkan_context::get()->get_logical_device();

		VkSamplerCreateInfo sampler_info(vk_base_sampler_create_info);
		sampler_info.magFilter = VK_FILTER_LINEAR;
		sampler_info.minFilter = VK_FILTER_LINEAR;
		sampler_info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		sampler_info.anisotropyEnable = VK_FALSE;
		sampler_info.maxAnisotropy = 1.0f;
		sampler_info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		sampler_info.unnormalizedCoordinates = VK_FALSE;
		sampler_info.compareEnable = VK_FALSE;
		sampler_info.compareOp = VK_COMPARE_OP_ALWAYS;
		sampler_info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		sampler_info.mipLodBias = 0.0f;
		sampler_info.minLod = 0.0f;
		sampler_info.maxLod = 0.0f;

		VK_CHECK_RESULT(vkCreateSampler(device->get_logical_device(), &sampler_info, nullptr, &texture2d_image._sampler));
	}

	void vulkan_texture2D::shutdown()
	{
		ref_ptr<vulkan_logical_device> device = vulkan_context::get()->get_logical_device();

		vkDestroySampler(device->get_logical_device(), texture2d_image._sampler, nullptr);
		vkDestroyImageView(device->get_logical_device(), texture2d_image._image_view, nullptr);

		vkDestroyImage(device->get_logical_device(), texture2d_image._image, nullptr);
		vkFreeMemory(device->get_logical_device(), texture2d_image._memory, nullptr);
	}

}



