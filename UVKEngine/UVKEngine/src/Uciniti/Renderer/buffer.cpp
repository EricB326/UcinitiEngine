#include "uvkpch.h"
#include "buffer.h"

#include "renderer_api.h"
#include "Uciniti/Platform/Vulkan/vulkan_buffer.h"

namespace Uciniti
{
	// =================================================================
	// Vertex Buffer
	// =================================================================
	vertex_buffer* vertex_buffer::create(void* a_data, uint32_t a_size)
	{
		switch (renderer_api::get_current_api())
		{
		case renderer_api_type::none: return nullptr;
		case renderer_api_type::vulkan: return new vulkan_vertex_buffer(a_data, a_size);
		}

		UVK_CORE_ASSERT(false, "Unknown Rendering API type.");
		return nullptr;
	}

	// =================================================================
	// Index Buffer
	// =================================================================
	index_buffer* index_buffer::create(void* a_data, uint32_t a_size /*= 0*/)
	{
		switch (renderer_api::get_current_api())
		{
		case renderer_api_type::none: return nullptr;
		case renderer_api_type::vulkan: return new vulkan_index_buffer(a_data, a_size);
		}

		UVK_CORE_ASSERT(false, "Unknown Rendering API type.");
		return nullptr;
	}
}