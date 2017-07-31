#version 430

float toDiscard;

uniform vec3 light_pos;

in vec4 color;
in vec3 centerPosition;

out vec4 out_color;

void main()
{
	out_color = color;
}