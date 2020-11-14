#ifndef VULKAN_RENDER_PASS_H
#define VULKAN_RENDER_PASS_H

#include "Uciniti/Renderer/render_pass.h"

namespace Uciniti
{
	class vulkan_render_pass : public render_pass
	{
	public:
		vulkan_render_pass(const render_pass_spec& a_spec);
		virtual ~vulkan_render_pass();

		virtual render_pass_spec& get_specification() override { return spec; }
		virtual const render_pass_spec& get_specification() const override { return spec; }

	private:
		render_pass_spec spec;
	};
}

#endif // !VULKAN_RENDER_PASS_H
