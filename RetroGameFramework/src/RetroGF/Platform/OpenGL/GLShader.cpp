#include "RGFpch.h"
#include "GLShader.h"

#include <GLAD/include/glad.h>

#include <glm/glm/gtc/type_ptr.hpp>
#include "RetroGF/FileSystem.h"

#include "GLCommon.h"

namespace RGF {

	Shader* Shader::Create() {
		return new GLShader;
	}

	GLShader::GLShader() {
		Init();
	}

	GLShader::~GLShader() {
		ShutDown();
	}


	void GLShader::Init() {
		GLCall(m_RendererID = glCreateProgram());
	}


	GLShader::ShaderSource GLShader::PraseShader(const std::string shaderFile) {
		if (!FileSystem::DoesFileExist(shaderFile)) {
			RGF_CORE_CRIT("File '%s' Does not exist!\n", shaderFile.c_str());
			__debugbreak();
		}
		enum class ShaderType {
			None = -1,
			Vertex, Fragment
		};
		ShaderType type = ShaderType::None;


		std::ifstream file(shaderFile);
		std::string line;
		std::stringstream ss[2];

		while (getline(file, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos) {
					type = ShaderType::Vertex;
				} else if (line.find("fragment") != std::string::npos) {
					type = ShaderType::Fragment;
				}
			}
			else {
				ss[(int)type] << line << "\n";
			}
		}

		return { ss[0].str(), ss[1].str() };
	}


	void GLShader::LoadFromSrc(const std::string& filepath) {
		m_Filepath = filepath;
		const auto& program = m_RendererID;
		ShaderSource source = PraseShader(filepath);

		unsigned int vs = CreateShader(GL_VERTEX_SHADER, source.VertexShaderStr);
		unsigned int fs = CreateShader(GL_FRAGMENT_SHADER, source.FragmentShaderStr);


		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));
		
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));
	}

	unsigned int GLShader::CreateShader(unsigned int type, const std::string& shaderSource) {
		unsigned int shader = glCreateShader(type);
		const char* shdSrc = shaderSource.c_str();
		
		GLCall(glShaderSource(shader, 1, &shdSrc, nullptr));
		GLCall(glCompileShader(shader));

		int validation;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &validation);

		if (!validation) {

			int logLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			char* message = (char*)alloca(logLength);
			GLCall(glGetShaderInfoLog(shader, logLength, &logLength, message));

			std::string shaderType = (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
			RGF_ERROR("OpenGL (%s) Error! MESSAGE :\n %s\n\n", shaderType.c_str(), message);

			GLCall(glDeleteShader(shader));
			return 0;
		}

		return shader;
	}



	void GLShader::Bind() const {
		GLCall(glUseProgram(m_RendererID));
	}
	void GLShader::Unbind() const {
		GLCall(glUseProgram(0));
	}


	void GLShader::ShutDown() {
		GLCall(glDeleteProgram(m_RendererID));
	}








	void GLShader::SetUniformMatrix(const std::string& uniformName, const glm::mat4& matrix) {
		int location = glGetUniformLocation(m_RendererID, uniformName.c_str());
		if (location == -1) {
			RGF_CORE_WARN("Uniform '%s' does not exist!", uniformName.c_str());
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}