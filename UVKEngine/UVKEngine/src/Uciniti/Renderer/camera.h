#ifndef CAMERA_H
#define CAMERA_H

#include "Uciniti/Renderer/base_camera.h"

namespace Uciniti
{
	class camera : public base_camera
	{
	public:
		enum class proj_type { perspective = 0, orthographic = 1 };

		camera();
		virtual ~camera();

		void set_perspective(float a_vertical_fov, float a_near_clip = 0.01f, float a_far_clip = 1000.0f);
		void set_orthographic(float a_size, float a_near_clip = -1.0f, float a_far_clip = 1.0f);
		void set_viewport_size(uint32_t a_width, uint32_t a_height);

		void set_perspective_vertical_fov(float a_vertical_fov) { perspective_fov = a_vertical_fov; }
		void set_perspective_near_clip(float a_near_clip) { perspective_near = a_near_clip; }
		void set_perspective_far_clip(float a_far_clip) { perspective_far = a_far_clip; }
		float get_perspective_vertical_fov() const { return perspective_fov; }
		float get_perspective_near_clip() const { return perspective_near; }
		float get_perspective_far_clip() const { return perspective_far; }

		void set_orthographic_size(float a_size) { orthographic_size = a_size; }
		void set_orthographic_near_clip(float a_near_clip) { orthographic_near = a_near_clip; }
		void set_orthographic_far_clip(float a_far_clip) { orthographic_far = a_far_clip; }
		float get_orthographic_size() const { return orthographic_size; }
		float get_orthographic_near_clip() const { return orthographic_near; }
		float get_orthographic_far_clip() const { return orthographic_far; }

		void set_projection_type(proj_type a_type) { projection_type = a_type; }
		proj_type get_projection_type() const { return projection_type; }

	private:
		proj_type projection_type = proj_type::perspective;

		float perspective_fov;
		float perspective_near = 0.01f, perspective_far = 1000.0f;

		float orthographic_size;
		float orthographic_near = -1.0f, orthographic_far = 1.0f;

	};
}

#endif // !CAMERA_H