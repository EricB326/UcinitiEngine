#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include "Uciniti/Renderer/framebuffer.h"

namespace Uciniti
{
	struct render_pass_spec
	{
		ref_ptr<framebuffer> target_framebuffer;
	};

	class render_pass
	{
	public:
		virtual ~render_pass() = default;

		virtual render_pass_spec& get_specification() = 0;
		virtual const render_pass_spec& get_specification() const = 0;

		static ref_ptr<render_pass> create(const render_pass_spec& a_spec);
	};
}

#endif // !RENDER_PASS_H