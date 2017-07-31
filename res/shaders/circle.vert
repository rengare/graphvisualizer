#version 430

in vec3 in_position;
in vec4 in_color;
in float in_size;

uniform mat4 projection_matrix, view_matrix;

out vec4 color;
out vec3 centerPosition;

void main()
{

	color = in_color;

	float scale = 500;
	float size = in_size;

    vec4 modelViewPosition = view_matrix * vec4(in_position, 1);
	
    gl_PointSize = size * (500 / -modelViewPosition.z);
	
	centerPosition = in_position;

	gl_Position = projection_matrix * modelViewPosition;
	
}