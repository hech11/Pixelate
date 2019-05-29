#shader vertex
#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aColor;
//layout(location = 2) in vec2 aTexCoords;

uniform mat4 u_Model = mat4(1.0f);
uniform mat4 u_View = mat4(1.0f);
uniform mat4 u_Proj = mat4(1.0f);

out DATA {
	vec4 position;
	vec4 color;
} vs_out;

void main() {
	gl_Position = u_Proj * u_View * u_Model * aPos;
	vs_out.position = u_Model * aPos;
	vs_out.color = aColor;
}

#shader fragment
#version 330 core


out vec4 Color;


in DATA {
	vec4 position;
	vec4 color;
} fs_in;

void main() {
	Color = fs_in.color;
}