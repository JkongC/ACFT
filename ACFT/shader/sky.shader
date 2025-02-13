#shader vertex
#version 420 core

layout(location = 0) in vec4 position;

out float distance_from_edge;

layout(std140, binding = 0) uniform basic
{
	mat4 view;
	mat4 proj;
	vec3 cam_pos;
};

void main()
{
	vec4 fixed_pos = position + vec4(cam_pos, 0.0);
	gl_Position = proj * view * fixed_pos;
	if (position.x == 0 && position.z == 0)
	{
		distance_from_edge = 1.0;
	}
	else
	{
		distance_from_edge = 0.0;
	}
}


#shader fragment
#version 420 core

layout(location = 0) out vec4 color;

in float distance_from_edge;

void main()
{
	vec4 faded_blue = vec4(0.788, 0.856, 1.0, 1.0);
	vec4 sky_blue = vec4(0.47, 0.64, 1.0, 1.0);
	color = mix(faded_blue, sky_blue, smoothstep(0.0, 1.0, distance_from_edge));
}