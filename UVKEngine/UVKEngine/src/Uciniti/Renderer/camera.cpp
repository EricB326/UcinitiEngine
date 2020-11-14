#include "uvkpch.h"
#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Uciniti
{
	camera::camera()
	{}

	camera::~camera()
	{}

	void camera::set_perspective(float a_vertical_fov, float a_near_clip /*= 0.01f*/, float a_far_clip /*= 1000.0f*/)
	{
		projection_type = proj_type::perspective;
		perspective_fov = a_vertical_fov;
		perspective_near = a_near_clip;
		perspective_far = a_far_clip;
	}

	void camera::set_orthographic(float a_size, float a_near_clip /*= -1.0f*/, float a_far_clip /*= 1.0f*/)
	{
		projection_type = proj_type::orthographic;
		orthographic_size = a_size;
		orthographic_near = a_near_clip;
		orthographic_far = a_far_clip;
	}

	void camera::set_viewport_size(uint32_t a_width, uint32_t a_height)
	{
		switch (projection_type)
		{
		case Uciniti::camera::proj_type::perspective:
			projection_matrix = glm::perspectiveFov(perspective_fov, (float)a_width, (float)a_height, perspective_near, perspective_far);
			break;
		case Uciniti::camera::proj_type::orthographic:
			float aspect = (float)a_width / (float)a_height;
			float width = orthographic_size * aspect;
			float height = orthographic_size;
			projection_matrix = glm::ortho(-width * 0.5f, width * 0.5f, -height * 0.5f, height * 0.5f);
			break;
		}
	}

}