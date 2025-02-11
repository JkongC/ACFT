#shader vertex
#version 420 core

layout(location = 0) in vec4 position;

out float box_height;

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
	box_height = (position.y + 80.0) / 160.0;
}


#shader fragment
#version 420 core

layout(location = 0) out vec4 color;

in float box_height;

void main()
{
	vec4 faded_white = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 sky_blue = vec4(0.47, 0.64, 1.0, 1.0);
	color = mix(sky_blue, faded_white, 0.4 * smoothstep(0.0, 0.16, 0.65 - box_height));
}