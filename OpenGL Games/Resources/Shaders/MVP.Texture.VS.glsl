#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 frag_normal;
out vec3 frag_color;
out vec2 tex_coord;

void main()
{
	frag_normal = v_normal;
	frag_color = v_color;
	tex_coord = v_texture_coord;
	gl_Position = projection * view * model * vec4(v_position, 1.0);
}
