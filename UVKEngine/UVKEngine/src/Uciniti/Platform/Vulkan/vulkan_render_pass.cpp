#include "uvkpch.h"
#include "vulkan_render_pass.h"

namespace Uciniti
{
	vulkan_render_pass::vulkan_render_pass(const render_pass_spec& a_spec)
		: spec(a_spec)
	{}

	vulkan_render_pass::~vulkan_render_pass()
	{}
}