#shader vertex

#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;


uniform mat4 u_ViewProj = mat4(1.0f);

out vec4 v_Color;
out vec2 v_UV;


void main() {
	v_Color = aColor;
	v_UV = aUV;
	gl_Position = u_ViewProj * aPos;
}



#shader fragment

#version 330 core

layout(location = 0) out vec4 FragColor;


in vec4 v_Color;
in vec2 v_UV;

void main() {
	FragColor = v_Color;
}
