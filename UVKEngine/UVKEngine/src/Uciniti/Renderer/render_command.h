#ifndef RENDER_COMMAND_H
#define RENDER_COMMAND_H

namespace Uciniti
{
	class render_command
	{
	public:
		typedef void(*render_command_fn)(void*);

		render_command();
		~render_command();

		void* allocate(render_command_fn func, uint32_t size);

		void execute();
	private:
		uint8_t* _command_buffer;
		uint8_t* _command_buffer_ptr;
		uint32_t _command_count = 0;

	};
}

#endif // !RENDER_COMMAND_H