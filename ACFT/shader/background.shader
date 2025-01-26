#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main(){
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

vec4 fog(vec4 prevColor){
	vec4 fogColor = vec4(0.6, 0.8, 0.9, 1.0);
	return mix(prevColor, fogColor, max((gl_FragCoord.z - 0.9) / 0.1, 0.0));
}

void main(){
	vec4 texColor = texture(u_Texture, v_TexCoord);
	texColor = fog(texColor);
	color = texColor;
}