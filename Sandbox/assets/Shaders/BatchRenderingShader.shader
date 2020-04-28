#shader vertex

#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;
layout(location = 3) in float aTexIndex;


uniform mat4 u_ViewProj = mat4(1.0f);

out vec4 v_Color;
out vec2 v_UV;
out float v_TexIndex;


void main() {
	v_Color = aColor;
	v_UV = aUV;
	v_TexIndex = aTexIndex;
	gl_Position = u_ViewProj * aPos;
}



#shader fragment

#version 330 core

layout(location = 0) out vec4 FragColor;


in vec4 v_Color;
in vec2 v_UV;
in float v_TexIndex;


uniform sampler2D u_Textures[16];

void main() {
	FragColor = texture(u_Textures[int(v_TexIndex)], v_UV) * v_Color;
}
