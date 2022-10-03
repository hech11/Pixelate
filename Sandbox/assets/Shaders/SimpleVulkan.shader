#shader vertex

#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

layout (location = 0) out vec2 v_TexCoord;

layout(binding = 0) uniform Camera
{
	mat4 u_ViewProj;
};

void main() {
	gl_Position = u_ViewProj * vec4(aPos, 1.0f);
	v_TexCoord = aTexCoord;
}



#shader fragment

#version 450 core

layout(location = 0) out vec4 FragColor;
layout(location = 0) in vec2 v_TexCoord;


layout(binding = 0) uniform sampler2D u_Textures[32];

void main() {
	FragColor = texture(u_Textures[5], v_TexCoord);
}
