#include "uvkpch.h"
#include "base_camera.h"

namespace Uciniti 
{
	base_camera::base_camera(const glm::mat4& a_proj_matrix)
		: projection_matrix(a_proj_matrix)
	{ }
}
