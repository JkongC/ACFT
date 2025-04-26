#shader vertex
#version 420 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_uv;

out vec2 v_TexCoord;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * position;
	v_TexCoord = texture_uv;
}


#shader fragment
#version 420 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

/*float get_viewspace_z()
{
	float z_ndc = gl_FragCoord.z * 2.0 - 1.0;
	return -numerator / (denominator_1 - z_ndc * denominator_2);
}

vec4 fog(vec4 prev_color)
{
	float depth = -get_viewspace_z();
	
	vec4 fog_color = vec4(0.576, 0.712, 1.0, 1.0);
	return mix(prev_color, fog_color, smoothstep(30.0, 36.0, depth));
}*/

void main()
{
	/*vec4 tex_color = texture(u_Texture, v_TexCoord);
	tex_color = fog(tex_color);
	color = tex_color;*/
	color = texture(u_Texture, v_TexCoord);
}