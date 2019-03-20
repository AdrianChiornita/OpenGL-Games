#version 330

in vec2 tex_coord;

uniform sampler2D u_texture;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = texture(u_texture, tex_coord);
	if(out_color.a < 0.9) discard;
}