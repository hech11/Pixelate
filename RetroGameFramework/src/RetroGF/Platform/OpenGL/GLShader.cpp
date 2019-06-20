#include "RGFpch.h"
#include "GLShader.h"

#include <GLAD/include/glad.h>

#include <glm/glm/gtc/type_ptr.hpp>
#include "RetroGF/Application.h"

#include "GLCommon.h"

namespace RGF {



	GLShader::GLShader() {
		Init();
	}

	GLShader::~GLShader() {
		ShutDown();
	}


	void GLShader::Init() {
		GLCall(m_RendererID = glCreateProgram());
	}


	GLShader::ShaderSource GLShader::m_PraseShader(const std::string shaderFile) {
		if (!Application::GetApp().GetFileIO().DoesFileExist(shaderFile)) {
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

	GLShader::ShaderSource GLShader::m_PraseShader(const char* data) {
		enum class ShaderType {
			None = -1,
			Vertex, Fragment
		};
		ShaderType type = ShaderType::None;
		std::string line;
		std::istringstream file(data);
		std::stringstream ss[2];

		while (getline(file, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos) {
					type = ShaderType::Vertex;
				}
				else if (line.find("fragment") != std::string::npos) {
					type = ShaderType::Fragment;
				}
			}
			else {
				ss[(int)type] << line << "\n";
			}
		}

		return { ss[0].str(), ss[1].str() };
	}

	// TODO: This code is duplication! need to refactor and redeign at some point.

	void GLShader::LoadFromSrc(const char* data) {
		const auto& program = m_RendererID;
		ShaderSource source = m_PraseShader(data);

		

		unsigned int vs = m_CreateShader(GL_VERTEX_SHADER, source.VertexShaderStr);
		unsigned int fs = m_CreateShader(GL_FRAGMENT_SHADER, source.FragmentShaderStr);


		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));

		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));
	}

	void GLShader::LoadFromFile(const std::string& filepath) {
		m_Filepath = filepath;
		const auto& program = m_RendererID;
		ShaderSource source = m_PraseShader(filepath);

		unsigned int vs = m_CreateShader(GL_VERTEX_SHADER, source.VertexShaderStr);
		unsigned int fs = m_CreateShader(GL_FRAGMENT_SHADER, source.FragmentShaderStr);


		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));
		
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));
	}

	unsigned int GLShader::m_CreateShader(unsigned int type, const std::string& shaderSource) {
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


	int GLShader::m_GetUniformLocation(const std::string& name) {

		if (m_CachedUniformLocations.find(name) != m_CachedUniformLocations.end()) {
			return m_CachedUniformLocations[name];
		}
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1) {
			RGF_CORE_WARN("Uniform '%s' does not exist!", name.c_str());
		}
		m_CachedUniformLocations[name] = location;
		return location;
	}


	void GLShader::SetUniform4f(const std::string& uniformName, const glm::vec4& values) {
		glUniform4f(m_GetUniformLocation(uniformName), values.x, values.y, values.z, values.w);
	}

	void GLShader::SetUniform1i(const std::string& uniformName, const int value) {
		glUniform1i(m_GetUniformLocation(uniformName), value);
	}


	void GLShader::SetUniformMatrix(const std::string& uniformName, const glm::mat4& matrix) {
		glUniformMatrix4fv(m_GetUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(matrix));
	}

}