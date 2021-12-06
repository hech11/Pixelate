#shader vertex

#version 450 core

layout(location = 0) in vec3 aPos;


layout(binding = 0) uniform Camera
{
	mat4 u_ViewProj;
};

void main() {
	gl_Position = u_ViewProj * vec4(aPos, 1.0f);
}



#shader fragment

#version 450 core

layout(location = 0) out vec4 FragColor;



void main() {
	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
