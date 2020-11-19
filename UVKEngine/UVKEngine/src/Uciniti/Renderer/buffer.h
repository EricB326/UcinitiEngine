#ifndef BUFFER_H
#define BUFFER_H

namespace Uciniti
{
	enum class shader_data_type
	{
		_none = 0, _float, _float2, _float3, _float4, _mat3, _mat4, _int, _int2, _int3, _int4, _bool
	};

	static uint32_t shader_data_type_size(shader_data_type a_type)
	{
		switch (a_type)
		{
		case shader_data_type::_float:  return 4;
		case shader_data_type::_float2: return 4 * 2;
		case shader_data_type::_float3: return 4 * 3;
		case shader_data_type::_float4: return 4 * 4;
		case shader_data_type::_mat3:   return 4 * 3 * 3;
		case shader_data_type::_mat4:   return 4 * 4 * 4;
		case shader_data_type::_int:	return 4;
		case shader_data_type::_int2:	return 4 * 2;
		case shader_data_type::_int3:	return 4 * 3;
		case shader_data_type::_int4:	return 4 * 4;
		case shader_data_type::_bool:	return 1;
		}

		UVK_CORE_ASSERT(false, "Unknown shader data type!");
		return 0;
	}

	struct buffer_element
	{
		std::string name;
		shader_data_type type;
		uint32_t size;
		uint32_t offset;
		bool normalised;

		buffer_element() = default;
		buffer_element(shader_data_type a_type, const std::string& a_name, bool a_normalised = false)
			: name(a_name), type(a_type), size(shader_data_type_size(a_type)), offset(0), normalised(a_normalised)
		{}

		uint32_t get_component_count() const
		{
			switch (type)
			{
			case shader_data_type::_float:  return 1;
			case shader_data_type::_float2: return 2;
			case shader_data_type::_float3: return 3;
			case shader_data_type::_float4: return 4;
			case shader_data_type::_mat3:	return 3 * 3;
			case shader_data_type::_mat4:	return 4 * 4;
			case shader_data_type::_int:	return 1;
			case shader_data_type::_int2:	return 2;
			case shader_data_type::_int3:	return 3;
			case shader_data_type::_int4:	return 4;
			case shader_data_type::_bool:	return 1;
			}

			UVK_CORE_ASSERT(false, "Unknown shader data type!");
			return 0;
		}
	};

	class buffer_layout
	{
	public:
		buffer_layout() {}

		buffer_layout(const std::initializer_list<buffer_element>& a_elements)
			: elements(a_elements)
		{
			calculate_offset_and_stride();
		}

		uint32_t get_stride() const { return stride; }
		const std::vector<buffer_element>& get_elements() const { return elements; }
		uint32_t get_element_count() const { return elements.size(); }

		std::vector<buffer_element>::iterator begin() { return elements.begin(); }
		std::vector<buffer_element>::iterator end() { return elements.end(); }
		std::vector<buffer_element>::const_iterator begin() const { return elements.begin(); }
		std::vector<buffer_element>::const_iterator eng() const { return elements.end(); }

	private:
		std::vector<buffer_element> elements;
		uint32_t stride;

		void calculate_offset_and_stride()
		{
			uint32_t offset = 0;
			stride = 0;

			for (auto& this_element : elements)
			{
				this_element.offset = offset;
				offset += this_element.size;
				stride += this_element.size;
			}
		}
	};

	class vertex_buffer
	{
	public:
		virtual ~vertex_buffer() {}

		virtual void set_data(void* a_buffer, uint32_t a_size, uint32_t a_offset = 0) = 0;
		virtual void bind() const = 0;

		virtual const buffer_layout& get_layout() const = 0;
		virtual void set_layout(const buffer_layout& a_layout) = 0;

		static vertex_buffer* create(void* a_data, uint32_t a_size, uint32_t a_element_count);

		virtual unsigned int get_data_size() const = 0;
		virtual unsigned int get_element_count() const = 0;

	private:

	};

	class index_buffer
	{
	public:
		virtual ~index_buffer() {}

		virtual void set_data(void* a_bufferm, uint32_t a_size, uint32_t a_offset = 0) = 0;
		virtual void bind() const = 0;

		virtual uint32_t get_count() const = 0;
		virtual uint32_t get_size() const = 0;

		static index_buffer* create(uint32_t a_size = 0);
		static index_buffer* create(void* a_data, uint32_t a_size = 0);

	private:

	};
}

#endif // !BUFFER_H