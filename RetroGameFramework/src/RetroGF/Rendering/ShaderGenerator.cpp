#include "RGFpch.h"
#include "ShaderGenerator.h"



namespace RGF {

	static const char* s_DefaultShader = R"(#shader vertex
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
uniform vec4 u_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);


in DATA {
	vec4 position;
	vec4 color;
} fs_in;

void main() {
	Color = fs_in.color * u_Color;
}
	)";
	





	static const char* s_TexturedShader = R"(#shader vertex
#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec4 aColor;

uniform mat4 u_Model = mat4(1.0f);
uniform mat4 u_View = mat4(1.0f);
uniform mat4 u_Proj = mat4(1.0f);

out DATA {
	vec4 position;
	vec4 color;
	vec2 uv;
} vs_out;

void main() {
	gl_Position = u_Proj * u_View * u_Model * aPos;
	vs_out.position = u_Model * aPos;
	vs_out.color = aColor;
	vs_out.uv = aUV;
}

#shader fragment
#version 330 core


out vec4 Color;
uniform sampler2D u_TextureSampler;

in DATA {
	vec4 position;
	vec4 color;
	vec2 uv;
} fs_in;

void main() {
	Color = texture2D(u_TextureSampler, fs_in.uv);
}
	)";
	
	ShaderGenerator* ShaderGenerator::s_Instance = nullptr;


	ShaderGenerator::ShaderGenerator() {
		s_Instance = this;
	}

	ShaderGenerator::~ShaderGenerator() {

	}


	Shader* ShaderGenerator::DefaultShader() {
		Shader* shader = nullptr;
		shader = Shader::Create();
		shader->LoadFromSrc(s_DefaultShader);
		return shader;
	}
	Shader* ShaderGenerator::TexturedShader() {
		Shader* shader = nullptr;
		shader = Shader::Create();
		shader->LoadFromSrc(s_TexturedShader);
		return shader;

	}

}