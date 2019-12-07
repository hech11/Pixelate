#include "RGFpch.h"
#include "GLShader.h"

#include <GLAD/include/glad.h>

#include <glm/glm/gtc/type_ptr.hpp>
#include "RetroGF/Core/Application.h"

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


	void GLShader::Bind() const {
		GLCall(glUseProgram(m_RendererID));
	}
	void GLShader::Unbind() const {
		GLCall(glUseProgram(0));
	}


	void GLShader::ShutDown() {
		GLCall(glDeleteProgram(m_RendererID));
	}

	GLShader::ShaderSource GLShader::PraseShader(const std::string& shaderFile) {
		std::string data = FileIO::Get().ReadFromStorage(shaderFile);
		if (data == "Error") {
			RGF_ASSERT(false, "Cannot find shader!\n");
		}
		return PraseShader(data.c_str());
	}

	GLShader::ShaderSource GLShader::PraseShader(const char* data) {
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
		ShaderSource source = PraseShader(data);

		

		unsigned int vs = CreateShader(GL_VERTEX_SHADER, source.VertexShaderStr);
		unsigned int fs = CreateShader(GL_FRAGMENT_SHADER, source.FragmentShaderStr);


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



	


	int GLShader::GetUniformLocation(const std::string& name) {
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



	void GLShader::SetUniform1f(const std::string& uniformName, const float value) {
		glUniform1f(GetUniformLocation(uniformName), value);
	}


	void GLShader::SetUniform2f(const std::string& uniformName, const glm::vec2& values) {
		glUniform2f(GetUniformLocation(uniformName), values.x, values.y);
	}


	void GLShader::SetUniform3f(const std::string& uniformName, const glm::vec3& values) {
		glUniform3f(GetUniformLocation(uniformName), values.x, values.y, values.z);
	}

	void GLShader::SetUniform4f(const std::string& uniformName, const glm::vec4& values) {
		glUniform4f(GetUniformLocation(uniformName), values.x, values.y, values.z, values.w);
	}



	void GLShader::SetUniform1i(const std::string& uniformName, const int value) {
		glUniform1i(GetUniformLocation(uniformName), value);
		
	}


	void GLShader::SetUniform2i(const std::string& uniformName, const glm::i32vec2& values) {
		glUniform2i(GetUniformLocation(uniformName), values.x, values.y);

	}


	void GLShader::SetUniform3i(const std::string& uniformName, const glm::i32vec3& values) {
		glUniform3i(GetUniformLocation(uniformName), values.x, values.y, values.z);

	}


	void GLShader::SetUniform4i(const std::string& uniformName, const glm::i32vec4& values) {
		glUniform4i(GetUniformLocation(uniformName), values.x, values.y, values.z, values.z);

	}



	void GLShader::SetUniform1i(const std::string& uniformName, const char value) {
		glUniform1i(GetUniformLocation(uniformName), value);

	}


	void GLShader::SetUniform2i(const std::string& uniformName, const glm::i8vec2& values) {
		glUniform2i(GetUniformLocation(uniformName), values.x, values.y);

	}


	void GLShader::SetUniform3i(const std::string& uniformName, const glm::i8vec3& values) {
		glUniform3i(GetUniformLocation(uniformName), values.x, values.y, values.z);

	}


	void GLShader::SetUniform4i(const std::string& uniformName, const glm::i8vec4& values) {
		glUniform4i(GetUniformLocation(uniformName), values.x, values.y, values.z, values.z);

	}


	void GLShader::SetUniformMatrix(const std::string& uniformName, const glm::mat4& matrix) {
		glUniformMatrix4fv(GetUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(matrix));

		//m_Uniforms.push_back({ uniformName, ShaderUnifromType::Mat4, location });
	}

}