// Vulkan triangle shader.

#stage vertex
#version 450 core

vec2 positions[3] = vec2[]
(
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colours[3] = vec3[]
(
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

layout(location = 0) out vec3 frag_colour;

void main() 
{
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
	frag_colour = colours[gl_VertexIndex];
}

#stage fragment
#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 frag_colour;

layout(location = 0) out vec4 out_colour;

void main() 
{
    out_colour = vec4(frag_colour, 1.0);
}