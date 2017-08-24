#version 430

in vec4 in_position;
in vec4 in_color;

uniform mat4 projection_matrix, view_matrix;

out vec4 color;
out vec3 centerPosition;

void main()
{

	color = in_color;
    vec4 modelViewPosition = view_matrix * vec4(in_position.xyz, 1);
	
	gl_Position = projection_matrix * modelViewPosition;
	
}