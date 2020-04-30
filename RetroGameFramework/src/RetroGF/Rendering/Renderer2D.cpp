#include "RGFpch.h"
#include "Renderer2D.h"


#include "RetroGF/Rendering/RenderingContext.h"
#include "RetroGF/Rendering/RenderCommand.h"

#include "RetroGF/Debug/Instrumentor.h"





namespace RGF {

	// Data that will be sent to the GPU.
	struct RGF_API QuadVertexData {
		glm::vec3 Verticies;
		unsigned int Color;
		glm::vec2 TextureCoords;
		float TextureIndex;
	};

	struct Renderer2DData {

		static const unsigned int MaxSprites = 20000;
		static const unsigned int MaxVerticiesSize = MaxSprites * 4;
		static const unsigned int MaxIndiciesSize = MaxSprites * 6;

		// TODO: this should be set dynamically as this differs between platforms and GPUs
		static const unsigned int MaxTextureSlots = 16;

		Ref<VertexArray> QuadVertexArray = nullptr;
		Ref<VertexBuffer> QuadVertexBuffer = nullptr;
		Ref<Shader> BatchRendererShader = nullptr;


		unsigned int IndexCount = 0;
		QuadVertexData* VertexDataBase = nullptr;
		QuadVertexData* VertexDataPtr = nullptr;


		std::array<glm::vec4, 4> QuadPivotPointPositions;
		std::array<glm::vec2, 4> TextureCoords;
		std::array<Ref<Texture>, MaxTextureSlots> AllTextureSlots;
		Ref<Texture> DefaultWhiteTexture;
		unsigned int TextureSlotIndex = 1; // 0 = white texture
		Renderer2D::RenderingStatistics m_Statistics;



	};

	static Renderer2DData SceneData;

	void Renderer2D::Init() {
		RGF_PROFILE_FUNCTION();


		{
			RGF_PROFILE_SCOPE("Renderer2D::Init::Setting-VertexBuffer");

			SceneData.m_Statistics.MaxIndexBuferSize = SceneData.MaxIndiciesSize;
			SceneData.m_Statistics.MaxVertexBufferSize = SceneData.MaxVerticiesSize;
			SceneData.m_Statistics.MaxSprites = SceneData.MaxSprites;

			SceneData.QuadVertexArray = VertexArray::Create();
			SceneData.QuadVertexBuffer = VertexBuffer::Create(SceneData.MaxVerticiesSize * sizeof(QuadVertexData));
		

			BufferLayout layout = 
			{
				{ BufferLayoutTypes::Float3, "aPos"},
				{ BufferLayoutTypes::UChar4, "aColor", true},
				{ BufferLayoutTypes::Float2, "aTexCoords"},
				{ BufferLayoutTypes::Float, "aTexIndex"}

			};

			SceneData.QuadVertexBuffer->SetLayout(layout);
			SceneData.QuadVertexArray->PushVertexBuffer(SceneData.QuadVertexBuffer);


			SceneData.VertexDataBase = new QuadVertexData[SceneData.MaxVerticiesSize];
		}

		{

			RGF_PROFILE_SCOPE("Renderer2D::Init::Setting-IndexBuffer");
			unsigned int* indices = new unsigned int[SceneData.MaxIndiciesSize];

			int offset = 0;
			for (unsigned int i = 0; i < SceneData.MaxIndiciesSize; i += 6) {
				indices[i] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;
				indices[i + 3] = offset + 2;

				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}

			Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, SceneData.MaxIndiciesSize);
			SceneData.QuadVertexArray->PushIndexBuffer(ibo);
			delete[] indices;

		}
		{

			RGF_PROFILE_SCOPE("Renderer2D::Init::Setting-Texture");

			SceneData.TextureCoords[0] = { 0.0f, 0.0f }; // -- bottom left
			SceneData.TextureCoords[1] = { 1.0f, 0.0f }; // -- bottom right
			SceneData.TextureCoords[2] = { 1.0f, 1.0f }; // -- top right
			SceneData.TextureCoords[3] = { 0.0f, 1.0f }; // -- top left


			SceneData.QuadPivotPointPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f }; // -- bottom left
			SceneData.QuadPivotPointPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f}; // -- bottom right
			SceneData.QuadPivotPointPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f}; // -- top right
			SceneData.QuadPivotPointPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f}; // -- top left

			SceneData.DefaultWhiteTexture = Texture::Create(1, 1, Texture::TextureProperties::Format::RGB);
			unsigned int whiteTextureData = 0xffffffff;
			SceneData.DefaultWhiteTexture->SetData(&whiteTextureData, 3);

			int samplers[SceneData.MaxTextureSlots];
			for (unsigned int i = 0; i < SceneData.MaxTextureSlots; i++)
				samplers[i] = i;

			{
				RGF_PROFILE_SCOPE("Renderer2D::Init::Setting-Shader");

				SceneData.BatchRendererShader = Shader::Create();
				SceneData.BatchRendererShader->LoadFromFile("assets/Shaders/BatchRenderingShader.shader");
				SceneData.BatchRendererShader->Bind();
				SceneData.BatchRendererShader->SetUniform1iArray("u_Textures", SceneData.MaxTextureSlots, samplers);
			}


			SceneData.AllTextureSlots[0] = SceneData.DefaultWhiteTexture;

		}


	}

	void Renderer2D::Begin(RGF::OrthographicCamera* camera) {
		RGF_PROFILE_FUNCTION();

		SceneData.BatchRendererShader->Bind();
		SceneData.BatchRendererShader->SetUniformMatrix("u_ViewProj", camera->GetViewProjectionMatrix());
		SceneData.IndexCount = 0;

		SceneData.TextureSlotIndex = 1;

		SceneData.VertexDataPtr = SceneData.VertexDataBase;

	}
	void Renderer2D::End() {
		RGF_PROFILE_FUNCTION();

		unsigned int size = (unsigned char*)SceneData.VertexDataPtr - (unsigned char*)SceneData.VertexDataBase;
		SceneData.QuadVertexBuffer->SetData(SceneData.VertexDataBase, size);

		SceneData.m_Statistics.VertexSize = size;

		Flush();
	}

	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color) {
		RGF_PROFILE_FUNCTION();
		DrawSprite(position, 0.0f, size, color);
	}


	void Renderer2D::DrawSprite(const glm::vec3& position, float rotation, const glm::vec3& size, const glm::vec4& color) {
		RGF_PROFILE_FUNCTION();

		constexpr unsigned int VertexCount = 4;


		if (SceneData.IndexCount >= SceneData.MaxIndiciesSize) {
			FlushAndBeginNewBatch();
		}

		unsigned char r = color.x * 255.0f;
		unsigned char g = color.y * 255.0f;
		unsigned char b = color.z * 255.0f;
		unsigned char a = color.w * 255.0f;


		unsigned int col = a << 24 | b << 16 | g << 8 | r;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), size);

		// Vertex order = bottom left -> bottom right -> top right -> top left
		for (unsigned int i = 0; i < VertexCount; i++) {

			SceneData.VertexDataPtr->Verticies = transform * SceneData.QuadPivotPointPositions[i];
			SceneData.VertexDataPtr->Color = col;
			SceneData.VertexDataPtr->TextureCoords = SceneData.TextureCoords[i];
			SceneData.VertexDataPtr->TextureIndex = 0.0f;
			SceneData.VertexDataPtr++;
		}


		SceneData.IndexCount += 6;
		SceneData.m_Statistics.IndexCount += 6;

	}


	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec3& size, const Ref<Texture>& texture, const glm::vec4& tintColor) {
		RGF_PROFILE_FUNCTION();
		DrawSprite(position, 0.0f, size, texture, tintColor);
	}



	void Renderer2D::DrawSprite(const glm::vec3& position, float rotation, const glm::vec3& size, const Ref<Texture>& texture, const glm::vec4& tintColor) {
		RGF_PROFILE_FUNCTION();

		constexpr unsigned int VertexCount = 4;


		unsigned char r = tintColor.r * 255.0f;
		unsigned char g = tintColor.g * 255.0f;
		unsigned char b = tintColor.b * 255.0f;
		unsigned char a = tintColor.a * 255.0f;

		unsigned int color = a << 24 | b << 16 | g << 8 | r;

		if (SceneData.IndexCount >= SceneData.MaxIndiciesSize) {
			FlushAndBeginNewBatch();
		}

		
		float textureIndex = 0.0f;

		for (unsigned int i = 1; i < SceneData.TextureSlotIndex; i++) {
			if (*SceneData.AllTextureSlots[i].get() == *texture.get()) {
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (SceneData.TextureSlotIndex >= SceneData.MaxTextureSlots)
				FlushAndBeginNewBatch();
			textureIndex = SceneData.TextureSlotIndex;
			SceneData.AllTextureSlots[SceneData.TextureSlotIndex] = texture;
			SceneData.TextureSlotIndex++;
		}



		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), size);


		// Vertex order = bottom left -> bottom right -> top right -> top left
		for (unsigned int i = 0; i < VertexCount; i++) {

			SceneData.VertexDataPtr->Verticies = transform * SceneData.QuadPivotPointPositions[i];
			SceneData.VertexDataPtr->Color = color;
			SceneData.VertexDataPtr->TextureCoords = SceneData.TextureCoords[i];
			SceneData.VertexDataPtr->TextureIndex = textureIndex;
			SceneData.VertexDataPtr++;
		}


		SceneData.IndexCount += 6;
		SceneData.m_Statistics.IndexCount += 6;
	}

	void Renderer2D::FlushAndBeginNewBatch() {
		RGF_PROFILE_FUNCTION();

		unsigned int size = (unsigned char*)SceneData.VertexDataPtr - (unsigned char*)SceneData.VertexDataBase;
		SceneData.QuadVertexBuffer->SetData(SceneData.VertexDataBase, size);

		Flush();

		SceneData.IndexCount = 0;
		SceneData.VertexDataPtr = SceneData.VertexDataBase;
		SceneData.TextureSlotIndex = 1;
	}

	void Renderer2D::Flush() {
		RGF_PROFILE_FUNCTION();

		for (unsigned int i = 0; i < SceneData.TextureSlotIndex; i++) {
			SceneData.AllTextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawElements(SceneData.QuadVertexArray, SceneData.IndexCount);
		SceneData.m_Statistics.DrawCalls += 1;


	}

	void Renderer2D::ResetStatistics() {
		RGF_PROFILE_FUNCTION();
		SceneData.m_Statistics.DrawCalls = 0;
		SceneData.m_Statistics.IndexCount = 0;
		SceneData.m_Statistics.VertexSize = 0;
	}

	RGF::Renderer2D::RenderingStatistics& Renderer2D::GetStats() {
		return SceneData.m_Statistics;
	}

	
}