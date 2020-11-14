#ifndef RENDERER_CONTEXT_H
#define RENDERER_CONTEXT_H

struct GLFWwindow;

namespace Uciniti
{
	class renderer_context
	{
	public:
		renderer_context() = default;
		virtual ~renderer_context() = default;

		virtual void init() = 0;
		virtual void begin_frame() = 0;
		virtual void swap_buffers() = 0;

		static ref_ptr<renderer_context> create(void* a_window);
		//static renderer_context* create(void* a_window);

	};
}

#endif // RENDERER_CONTEXT_H
