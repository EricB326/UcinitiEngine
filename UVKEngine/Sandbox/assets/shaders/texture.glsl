// Base texture shader.

#stage vertex
#version 450 core

layout(location = 0) in vec3 _position;
layout(location = 1) in vec2 _tex_coord;

layout(location = 0) out vec2 _final_tex_coord;

void main() 
{
	vec4 final_position = vec4(_position.xy, 0.0, 1.0);
	_final_tex_coord = _tex_coord;
	gl_Position = final_position;
}

#stage fragment
#version 450 core

layout (location = 0) in vec2 _final_tex_coord;

layout(binding = 0) uniform sampler2D _texture;

layout(location = 0) out vec4 _final_colour;

void main() 
{
    _final_colour = texture(_texture, _tex_coord);
}