#ifndef VULKAN_BUFFER_H
#define VULKAN_BUFFER_H

#include "Uciniti/Renderer/buffer.h"

namespace Uciniti
{

	class vulkan_vertex_buffer : public vertex_buffer
	{
	public:
		vulkan_vertex_buffer(void* a_data, uint32_t a_size);
		virtual ~vulkan_vertex_buffer() {}

		virtual void set_data(void* a_buffer, uint32_t a_size, uint32_t a_offset = 0) override;
		virtual void bind() const override;

		virtual const buffer_layout& get_layout() const override { return {}; }
		virtual void set_layout(const buffer_layout& a_layout) override {}

	private:

	};

	class vulkan_index_buffer : public index_buffer
	{
	public:
		vulkan_index_buffer(uint32_t a_size);
		vulkan_index_buffer(void* a_data, uint32_t a_size);
		virtual ~vulkan_index_buffer() {}

		virtual void set_data(void* a_bufferm, uint32_t a_size, uint32_t a_offset = 0) override;
		virtual void bind() const override;

		virtual uint32_t get_count() const override { return buffer_size / sizeof(uint32_t); }
		virtual uint32_t get_size() const override { return buffer_size; }

	private:
		uint32_t buffer_size = 0;
	};
}
#endif // !VULKAN_BUFFER_H