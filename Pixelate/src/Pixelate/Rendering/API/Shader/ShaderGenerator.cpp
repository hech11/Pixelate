#include "PXpch.h"
#include "ShaderGenerator.h"


namespace Pixelate {


	static const char* s_DefaultShader = R"(#shader vertex
#version 330 core

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;


uniform mat4 u_Model = mat4(1.0f);
uniform mat4 u_ViewProj = mat4(1.0f);

out DATA {
	vec4 position;
	vec4 color;
	vec2 uv;
} vs_out;

void main() {
	gl_Position = u_ViewProj * u_Model * aPos;
	vs_out.position = u_Model * aPos;
	vs_out.color = aColor;
	vs_out.uv = aUV;
}

#shader fragment
#version 330 core


out vec4 Color;
uniform vec4 u_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
uniform sampler2D u_TextureSampler;
uniform bool u_IsTexture = true;

in DATA {

	vec4 position;
	vec4 color;
	vec2 uv;

} fs_in;

void main() {
	if(u_IsTexture) {
		Color = texture2D(u_TextureSampler, fs_in.uv);
	}
	else {
		Color = u_Color;
	}
}
	)";






	ShaderGenerator* ShaderGenerator::s_Instance = nullptr;
	ShaderGenerator::ShaderGenerator() {
		s_Instance = this;
	}

	ShaderGenerator::~ShaderGenerator() {

	}


	Ref<Shader> ShaderGenerator::DefaultShader() {
		Ref<Shader> shader = Shader::Create();
		shader->LoadFromSrc(s_DefaultShader);
		return shader;
	}


}

