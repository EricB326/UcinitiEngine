#ifndef BASE_CAMERA_H
#define BASE_CAMERA_H

#include <glm/glm.hpp>

namespace Uciniti
{
	class base_camera
	{
	public:
		base_camera() = default;
		base_camera(const glm::mat4& a_proj_matrix);
		virtual ~base_camera() = default;
		
		const glm::mat4& get_proj_matrix() const { return projection_matrix; }
		void set_proj_matrix(const glm::mat4& a_proj_matrix) { projection_matrix = a_proj_matrix; }

		/* @brief Return a const unmodifiable reference of the exposure.
		*/
		const float& get_exposure() const { return exposure; }

		/* @brief Return a modifiable reference of the exposure.
		*/
		float& get_exposure() { return exposure; }

	protected:
		glm::mat4 projection_matrix = glm::mat4(1.0f);
		float exposure = 0.8f;
	};
}
	
#endif // !BASE_CAMERA_H