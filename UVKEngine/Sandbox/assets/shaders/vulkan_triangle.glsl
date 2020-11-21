// Vulkan triangle shader.

#stage vertex
#version 450 core

layout(location = 0) in vec3 in_vert_position;
layout(location = 1) in vec3 in_vert_colour;
layout(location = 2) in vec2 in_vert_tex_coord;

layout(binding = 0) uniform in_vert_mvp
{
	mat4 model;
	mat4 view;
	mat4 proj;
} mvp;

layout(location = 0) out vec3 out_frag_colour;
layout(location = 1) out vec2 out_frag_tex_coord;

void main()
{
	gl_Position = mvp.proj * mvp.view * mvp.model * vec4(in_vert_position, 1.0);
	out_frag_colour = in_vert_colour;
	out_frag_tex_coord = in_vert_tex_coord;
}

#stage fragment
#version 450 core

layout(location = 0) in vec3 in_frag_colour;
layout(location = 1) in vec2 in_frag_tex_coord;

layout(binding = 1) uniform sampler2D in_frag_tex_sampler;

layout(location = 0) out vec4 out_final_colour;

void main() 
{
	out_final_colour = texture(in_frag_tex_sampler, in_frag_tex_coord);
}