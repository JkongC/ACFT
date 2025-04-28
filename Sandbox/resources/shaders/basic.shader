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

void main()
{
	color = texture(u_Texture, v_TexCoord);
}