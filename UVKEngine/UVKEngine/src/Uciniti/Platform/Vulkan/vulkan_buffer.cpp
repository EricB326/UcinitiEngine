#include "uvkpch.h"
#include "vulkan_buffer.h"

namespace Uciniti
{
	// =================================================================
	// Vulkan Vertex Buffer
	// =================================================================
	vulkan_vertex_buffer::vulkan_vertex_buffer(void* a_data, uint32_t a_size)
	{

	}

	void vulkan_vertex_buffer::set_data(void* a_buffer, uint32_t a_size, uint32_t a_offset /*= 0*/)
	{

	}

	void vulkan_vertex_buffer::bind() const
	{

	}

	// =================================================================
	// Vulkan Index Buffer
	// =================================================================
	vulkan_index_buffer::vulkan_index_buffer(uint32_t a_size)
		: buffer_size(a_size)
	{

	}

	vulkan_index_buffer::vulkan_index_buffer(void* a_data, uint32_t a_size)
		: buffer_size(a_size)
	{

	}

	void vulkan_index_buffer::set_data(void* a_bufferm, uint32_t a_size, uint32_t a_offset /*= 0*/)
	{

	}

	void vulkan_index_buffer::bind() const
	{

	}

}