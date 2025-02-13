#shader vertex
#version 420 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

layout(std140, binding = 0) uniform basic
{
	mat4 view;
	mat4 proj;
	vec3 cam_pos;
};

void main()
{
	gl_Position = proj * view * position;
	v_TexCoord = texCoord;
}


#shader fragment
#version 420 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

uniform float numerator;
uniform float denominator_1;
uniform float denominator_2;

float get_viewspace_z()
{
	float z_ndc = gl_FragCoord.z * 2.0 - 1.0;
	return -numerator / (denominator_1 - z_ndc * denominator_2);
}

vec4 fog(vec4 prev_color)
{
	vec4 fog_color = vec4(0.788, 0.856, 1.0, 1.0);
	return mix(fog_color, prev_color, smoothstep(-26.0, -20.0, get_viewspace_z()));
}

void main()
{
	vec4 tex_color = texture(u_Texture, v_TexCoord);
	tex_color = fog(tex_color);
	color = tex_color;
}