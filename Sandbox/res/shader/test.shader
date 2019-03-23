#shader vertex
#version 330 core

layout(location = 0) in vec4 aPos;

void main() {
	gl_Position = aPos;
}

#shader fragment
#version 330 core

out vec4 Color;

void main() {
	Color = vec4(1.0f, 0.0f, 1.0f, 1.0f);
}