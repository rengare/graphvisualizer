#version 430

float toDiscard;

uniform vec3 light_pos;

in vec4 color;
in vec3 centerPosition;

out vec4 out_color;

void main()
{
	toDiscard = 0.5;
	vec2 coord = gl_PointCoord - vec2(toDiscard);
	
	if(length(coord) > toDiscard){
		discard;
	}else{
		float distance = length(centerPosition - light_pos);
		float intensity;
		if(distance < 500){
			intensity = 1;
		}else{
			intensity = 0.1;
		}
		// float intensity = 1;
		// out_color = vec4(color.rgb * intensity, 1);
		out_color = color;
	}
}