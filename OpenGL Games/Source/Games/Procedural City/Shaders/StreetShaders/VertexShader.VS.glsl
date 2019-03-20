#version 330

// get vertex attributes from each location
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture_coordinate;
layout(location = 3) in vec3 vertex_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// output values to fragment shader
out vec4 fragment_position;
out vec3 fragment_normal;
out vec2 fragment_texture_coordinate;

out vec3 world_position;
out vec3 world_normal;


void main()
{
	gl_Position = Projection * View * Model * vec4(vertex_position, 1.0);

	fragment_position = gl_Position;
	fragment_normal = vertex_normal;
	fragment_texture_coordinate = vertex_texture_coordinate;

	world_position = (Model * vec4(vertex_position, 1)).xyz;
	world_normal = normalize(mat3(Model) * vertex_normal);
}
