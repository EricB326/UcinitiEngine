#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glm/glm.hpp>

#include "Uciniti/Renderer/renderer_api.h"

namespace Uciniti
{
	struct framebuffer_spec
	{
		uint32_t width = 1280;
		uint32_t height = 720;
		uint32_t samples = 1; // samples for multi sampling.

		glm::vec4 clear_colour;

		/* @brief Specifies if this framebuffer is supposed to be rendering to the
				  swap chain. Allows to create a framebuffer that doesn't present.
		*/
		bool swap_chain_target = false;
	};


	class framebuffer
	{
	public:
		
		virtual const framebuffer_spec& get_specification() const = 0;

		static ref_ptr<framebuffer> create(const framebuffer_spec& a_spec);
	private:

	};
}

#endif // !FRAMEBUFFER_H