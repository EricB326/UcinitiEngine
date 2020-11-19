// Vulkan triangle shader.

#stage vertex
#version 450 core

layout(location = 0) in vec2 in_vert_position;
layout(location = 1) in vec3 in_vert_colour;

layout(binding = 0) uniform in_vert_mvp
{
	mat4 model;
	mat4 view;
	mat4 proj;
} mvp;

layout(location = 0) out vec3 out_vert_colour;

void main()
{
	gl_Position = mvp.proj * mvp.view * mvp.model * vec4(in_vert_position, 0.0, 1.0);
	out_vert_colour = in_vert_colour;
}

#stage fragment
#version 450 core

layout(location = 0) in vec3 in_frag_colour;

layout(location = 0) out vec4 out_frag_colour;

void main() 
{
	out_frag_colour = vec4(in_frag_colour, 1.0);
}