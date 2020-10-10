#ifndef RENDERER_CONTEXT_H
#define RENDERER_CONTEXT_H

namespace Uciniti
{
	class renderer_context
	{
	public:
		renderer_context() = default;
		virtual ~renderer_context() = default;

		virtual void create() = 0;
		virtual void swap_buffers() = 0;

	};
}

#endif // RENDERER_CONTEXT_H
