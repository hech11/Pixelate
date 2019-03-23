#pragma once

#include "RetroGF/Rendering/API/Shader.h"


namespace RGF {

	class RGF_API GLShader : public Shader {

		public:
			struct ShaderSource {
				std::string VertexShaderStr;
				std::string FragmentShaderStr;
			};

			GLShader();
			~GLShader();

			void Init() override;
			void ShutDown() override;



			void Bind() const override;
			void Unbind() const override;

			void LoadFromSrc(const std::string& filepath) override;

		private :
			unsigned int m_RendererID;

			std::string m_Filepath;
		private :
			ShaderSource PraseShader(const std::string shaderFile);
			unsigned int CreateShader(unsigned int type, const std::string& shaderSource);
	};

}