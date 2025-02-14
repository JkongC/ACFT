#shader vertex
#version 420 core

layout(location = 0) in vec4 position;

out float distance_from_edge;

layout(std140, binding = 0) uniform basic
{
	mat4 view;
	mat4 proj;
	vec3 cam_pos;
	float numerator;
	float denominator_1;
	float denominator_2;
};

void main()
{
	vec4 fixed_pos = position + vec4(cam_pos, 0.0);
	gl_Position = proj * view * fixed_pos;
	distance_from_edge = (position.x == 0 && position.z == 0) ? 1.0 : 0.0;
}


#shader fragment
#version 420 core

layout(location = 0) out vec4 color;

layout(std140, binding = 0) uniform basic
{
	mat4 view;
	mat4 proj;
	vec3 cam_pos;
	float numerator;
	float denominator_1;
	float denominator_2;
};

in float distance_from_edge;

float get_viewspace_z()
{
	float z_ndc = gl_FragCoord.z * 2.0 - 1.0;
	return -numerator / (denominator_1 - z_ndc * denominator_2);
}

vec4 fog(vec4 prev_color)
{
	vec4 fog_color = vec4(1.0, 1.0, 1.0, 1.0);
	float fog_factor = 0.2;
	float depth = -get_viewspace_z();

	if (depth > 30.0)
		return mix(prev_color, fog_color, fog_factor * smoothstep(30.0, 36.0, depth));
	else
		return prev_color;
}

void main()
{
	vec4 sky_blue = vec4(0.47, 0.64, 1.0, 1.0);
	color = fog(sky_blue);
}