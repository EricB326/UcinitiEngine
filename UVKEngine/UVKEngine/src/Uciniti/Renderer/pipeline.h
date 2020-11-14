#ifndef PIPELINE_H
#define PIPELINE_H

#include "Uciniti/Renderer/shader.h"
#include "Uciniti/Renderer/render_pass.h"

namespace Uciniti
{
	struct pipeline_spec
	{
		ref_ptr<shader> _shader;
		ref_ptr<render_pass> _render_pass;
		// Should also hold vertex layout data.
	};

	class pipeline
	{
	public:
		virtual ~pipeline() = default;

		virtual pipeline_spec& get_specification() = 0;
		virtual const pipeline_spec& get_specification() const = 0;

		virtual void init() = 0;

		static ref_ptr<pipeline> create(const pipeline_spec& a_spec);

	};
}

#endif // !PIPELINE_H