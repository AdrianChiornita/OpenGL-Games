#version 330 core
layout (location = 0) in vec3 vertex_position;

out vec3 texture_coordinates;

uniform mat4 Projection;
uniform mat4 View;
uniform vec3 eye_position;

void main()
{
    texture_coordinates = vertex_position;
	vec4 new_position = Projection * View * vec4(vertex_position + eye_position, 1.0);
    gl_Position = new_position.xyww;
}  