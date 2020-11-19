#ifndef VULKAN_BUFFER_H
#define VULKAN_BUFFER_H

#include "Uciniti/Platform/Vulkan/vulkan_base.h"
#include "Uciniti/Renderer/buffer.h"
#include "Uciniti/Platform/Vulkan/vulkan_context.h"


namespace Uciniti
{
	enum class vulkan_buffer_type
	{
		VERTEX = 0, INDEX, STAGING, UNIFORM 
	};

	class vulkan_buffer_util
	{
	public:
		static void create_buffer(vulkan_buffer_type a_buffer_type, VkBuffer& a_buffer, VkDeviceMemory& a_device_mem, VkDeviceSize a_buffer_size);
		static void copy_buffer(VkBuffer a_src_buffer, VkBuffer a_dst_buffer, VkDeviceSize a_buffer_size);
	};

	class vulkan_vertex_buffer : public vertex_buffer
	{
	public:
		vulkan_vertex_buffer(void* a_data, uint32_t a_size, uint32_t a_element_count);
		virtual ~vulkan_vertex_buffer();

		void* get_data() { return _stored_data; }
		virtual void set_data(void* a_buffer, uint32_t a_size, uint32_t a_offset = 0) override;
		virtual void bind() const override;

		virtual const buffer_layout& get_layout() const override { return {}; }
		virtual void set_layout(const buffer_layout& a_layout) override {}

		virtual unsigned int get_data_size() const override { return _buffer_size; }
		virtual unsigned int get_element_count() const override { return _element_count; }
		VkBuffer get_vertex_buffer() { return _vertex_buffer; }

		void shutdown();

	private:
		const vulkan_logical_device* _device;

		uint32_t _buffer_size = 0;
		uint32_t _element_count = 0;
		void* _stored_data;

		VkBuffer _vertex_buffer;
		VkDeviceMemory _device_memory;

		void create_vertex_buffer();
	};

	class vulkan_index_buffer : public index_buffer
	{
	public:
		vulkan_index_buffer(uint32_t a_size);
		vulkan_index_buffer(void* a_data, uint32_t a_size);
		virtual ~vulkan_index_buffer();

		virtual void set_data(void* a_bufferm, uint32_t a_size, uint32_t a_offset = 0) override;
		virtual void bind() const override;

		virtual uint32_t get_count() const override { return _buffer_size / sizeof(uint32_t); }
		virtual uint32_t get_size() const override { return _buffer_size; }
		VkBuffer get_index_buffer() { return _index_buffer; }

		void shutdown();

	private:
		const vulkan_logical_device* _device;

		uint32_t _buffer_size = 0;
		void* _stored_data;

		VkBuffer _index_buffer;
		VkDeviceMemory _device_memory;

		void create_index_buffer();
	};
}
#endif // !VULKAN_BUFFER_H