#include "uvkpch.h"
#include "buffer.h"

#include "renderer_api.h"
#include "Uciniti/Platform/Vulkan/vulkan_buffer.h"

namespace Uciniti
{
	// =================================================================
	// Vertex Buffer
	// =================================================================
	ref_ptr<vertex_buffer> vertex_buffer::create(void* a_data, uint32_t a_buffer_size, uint32_t a_element_count)
	{
		switch (renderer_api::get_current_api())
		{
			case renderer_api_type::none: return nullptr;
			case renderer_api_type::vulkan: return create_ref_ptr<vulkan_vertex_buffer>(a_data, a_buffer_size, a_element_count);
		}

		UVK_CORE_ASSERT(false, "Unknown Rendering API type.");
		return nullptr;
	}

	// =================================================================
	// Index Buffer
	// =================================================================
	ref_ptr<index_buffer> index_buffer::create(void* a_data, uint32_t a_size /*= 0*/)
	{
		switch (renderer_api::get_current_api())
		{
			case renderer_api_type::none: return nullptr;
			case renderer_api_type::vulkan: return create_ref_ptr<vulkan_index_buffer>(a_data, a_size);
		}

		UVK_CORE_ASSERT(false, "Unknown Rendering API type.");
		return nullptr;
	}
}