#ifndef RENDERER_API_H
#define RENDERER_API_H

#include <glm/glm.hpp>

namespace Uciniti
{
	enum class primitive_type
	{
		none = 0, triangles, lines
	};


	enum class renderer_api_type
	{
		none = 0, vulkan
	};

	class renderer_api
	{
	public:

		static void clear(float a_red, float a_green, float a_blue, float a_alpha);
		static void clear(const glm::vec4& a_clear_colour);
		static void set_api(renderer_api_type a_api);
		static void set_clear_colour(float a_red, float a_green, float a_blue, float a_alpha);
		static void set_clear_colour(const glm::vec4& a_clear_colour);

		static void draw_indexed(uint32_t a_count, primitive_type a_type, bool a_depth_test = true);

		inline static renderer_api_type get_current_api() { return current_renderer_api; }

	private:
		static renderer_api_type current_renderer_api;
	};

}

#endif // !RENDERER_API_H