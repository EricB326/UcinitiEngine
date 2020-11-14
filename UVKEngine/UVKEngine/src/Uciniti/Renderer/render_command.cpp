#include "uvkpch.h"
#include "render_command.h"

namespace Uciniti
{
	render_command::render_command()
	{
		_command_buffer = new uint8_t[10 * 1024 * 1024]; // 10mb buffer.
		_command_buffer_ptr = _command_buffer;
		memset(_command_buffer, 0, 10 * 1024 * 1024);
	}

	render_command::~render_command()
	{
		delete[] _command_buffer;
	}

	void* render_command::allocate(render_command_fn func, uint32_t size)
	{
		// #TODO: alignment
		*(render_command_fn*)_command_buffer_ptr = func;
		_command_buffer_ptr += sizeof(render_command_fn);

		*(uint32_t*)_command_buffer_ptr = size;
		_command_buffer_ptr += sizeof(uint32_t);

		void* memory = _command_buffer_ptr;
		_command_buffer_ptr += size;

		_command_count++;
		return memory;

	}

	void render_command::execute()
	{
		byte* buffer = _command_buffer;

		for (uint32_t i = 0; i < _command_count; i++)
		{
			render_command_fn function = *(render_command_fn*)buffer;
			buffer += sizeof(render_command_fn);

			uint32_t size = *(uint32_t*)buffer;
			buffer += sizeof(uint32_t);
			function(buffer);
			buffer += size;
		}

		_command_buffer_ptr = _command_buffer;
		_command_count = 0;
	}

}
