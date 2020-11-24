// Vulkan triangle shader.

#stage vertex
#version 450 core

layout(location = 0) in vec3 in_vert_position;
layout(location = 1) in vec3 in_vert_normal;
layout(location = 2) in vec3 in_vert_tangent;
layout(location = 3) in vec3 in_vert_binormal;
layout(location = 4) in vec2 in_vert_tex_coord;

layout(binding = 0) uniform in_vert_mvp
{
	mat4 model;
	mat4 view;
	mat4 proj;
} mvp;

struct vertex_output
{
	vec3 position;
	vec3 normal;
	vec2 tex_coord;
	//mat3 world_normals;
	//mat3 world_transform;
	//vec3 binormal;
};

layout(location = 0) out vertex_output out_frag_data;

void main()
{
	out_frag_data.position = in_vert_position;
	out_frag_data.normal = in_vert_normal;
	out_frag_data.tex_coord = in_vert_tex_coord;

	gl_Position = mvp.proj * mvp.view * mvp.model * vec4(in_vert_position, 1.0);
}

#stage fragment
#version 450 core

struct vertex_output
{
	vec3 position;
	vec3 normal;
	vec2 tex_coord;
	//mat3 world_normals;
	//mat3 world_transform;
	//vec3 binormal;
};

layout(location = 0) in vertex_output in_frag_data;

layout(binding = 1) uniform sampler2D in_frag_albedo_tex;

layout(location = 0) out vec4 out_final_colour;

void main() 
{
	out_final_colour = texture(in_frag_albedo_tex, in_frag_data.tex_coord);
}