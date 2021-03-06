#version 430

layout(location=0) in vec4 in_position;
layout(location=1) in vec4 in_color;
layout(location=2) in float in_size;

uniform mat4 projection_matrix, view_matrix;

out vec4 color;
out vec3 centerPosition;

void main()
{
	color = in_color;

	float scale = 500;
	float size = in_size;

	vec3 normalPosition = vec3(in_position);

    vec4 modelViewPosition = view_matrix * vec4(normalPosition, 1);
	
    gl_PointSize = size * (500 / -modelViewPosition.z);
	
	centerPosition = normalPosition;

	gl_Position = projection_matrix * modelViewPosition;
	
}