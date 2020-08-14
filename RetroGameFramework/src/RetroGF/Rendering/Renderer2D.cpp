#include "RGFpch.h"
#include "Renderer2D.h"


#include "RetroGF/Rendering/RendererAPI.h"
#include "RetroGF/Rendering/RenderCommand.h"

#include "RetroGF/Debug/Instrumentor.h"





namespace RGF {

	// Data that will be sent to the GPU.
	struct RGF_API SpriteVertexData {
		glm::vec3 Verticies;
		unsigned int Color;
		glm::vec2 TextureCoords;
		float TextureIndex;
	};


	struct RGF_API LineVertexData {
		glm::vec3 Verticies;
		unsigned int Color;
		glm::vec2 TextureCoords;
		float TextureIndex;
	};

	struct Renderer2DData {

		static const unsigned int MaxSprites = 20000;
		static const unsigned int MaxVerticiesSize = MaxSprites * 4;
		static const unsigned int MaxIndiciesSize = MaxSprites * 6;

		static const unsigned int MaxLines = 10000;
		static const unsigned int MaxLineVerticesSize = MaxLines * 2;
		static const unsigned int MaxLineIndicesSize = MaxLines * 6;


		// TODO: this should be set dynamically as this differs between platforms and GPUs
		static const unsigned int MaxTextureSlots = 16;


		Ref<VertexArray> SpriteVertexArray;
		Ref<VertexBuffer> SpriteVertexBuffer;
		Ref<Shader> BatchRendererShader;

		unsigned int SpriteIndexCount = 0;
		SpriteVertexData* SpriteVertexDataBase = nullptr;
		SpriteVertexData* SpriteVertexDataPtr = nullptr;

		// For rendering lines
		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		unsigned int LineIndexCount = 0;

		LineVertexData* LineVertexDataBase = nullptr;
		LineVertexData* LineQuadVertexData = nullptr;


		// For drawing the scene grid.
		Ref<VertexArray> SceneGridVertexArray;
		Ref<VertexBuffer> SceneGridVertexBuffer;
		Ref<Shader> SceneGridShader;

		std::array<glm::vec4, 4> QuadPivotPointPositions;
		std::array<glm::vec2, 4> TextureCoords;
		std::array<Ref<Texture>, MaxTextureSlots> AllTextureSlots;
		Ref<Texture> DefaultWhiteTexture;
		unsigned int TextureSlotIndex = 1; // 0 = white texture
		Renderer2D::RenderingStatistics m_Statistics;


		glm::mat4 m_ViewMatrix;
		//temp
		int Samplers[MaxTextureSlots];

		bool DrawBoundingBoxes = false;
	};

	static Renderer2DData SceneData;

	void Renderer2D::Init() {
		RGF_PROFILE_FUNCTION();


		{
			RGF_PROFILE_SCOPE("Renderer2D::Init::Setting-VertexBuffer");

			SceneData.m_Statistics.MaxIndexBuferSize = SceneData.MaxIndiciesSize;
			SceneData.m_Statistics.MaxVertexBufferSize = SceneData.MaxVerticiesSize;
			SceneData.m_Statistics.MaxSprites = SceneData.MaxSprites;

			SceneData.SpriteVertexArray = VertexArray::Create();
			SceneData.SpriteVertexBuffer = VertexBuffer::Create(SceneData.MaxVerticiesSize * sizeof(SpriteVertexData));
		

			BufferLayout layout = 
			{
				{ BufferLayoutTypes::Float3, "aPos"},
				{ BufferLayoutTypes::UChar4, "aColor", true},
				{ BufferLayoutTypes::Float2, "aTexCoords"},
				{ BufferLayoutTypes::Float, "aTexIndex"}

			};

			SceneData.SpriteVertexBuffer->SetLayout(layout);
			SceneData.SpriteVertexArray->PushVertexBuffer(SceneData.SpriteVertexBuffer);

			SceneData.SpriteVertexDataBase = new SpriteVertexData[SceneData.MaxVerticiesSize];

			// For lines
			SceneData.LineVertexArray = VertexArray::Create();
			SceneData.LineVertexBuffer = VertexBuffer::Create(SceneData.MaxLineVerticesSize * sizeof(LineVertexData));


			SceneData.LineVertexBuffer->SetLayout(layout);
			SceneData.LineVertexArray->PushVertexBuffer(SceneData.LineVertexBuffer);


			SceneData.LineVertexDataBase = new LineVertexData[SceneData.MaxLineVerticesSize];

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
			SceneData.SpriteVertexArray->PushIndexBuffer(ibo);
			SceneData.SpriteVertexArray->Unbind();
			delete[] indices;

			// for rendering lines
			unsigned int* lineIndices = new unsigned int[SceneData.MaxLineIndicesSize];
			for (unsigned int i = 0; i < SceneData.MaxLineIndicesSize; i++) {
				lineIndices[i] = i;
			}

			Ref<IndexBuffer> lineIbo = IndexBuffer::Create(lineIndices, SceneData.MaxLineIndicesSize);
			SceneData.LineVertexArray->PushIndexBuffer(lineIbo);

			SceneData.LineVertexArray->Unbind();
			delete[] lineIndices;
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


			for (unsigned int i = 0; i < SceneData.MaxTextureSlots; i++)
				SceneData.Samplers[i] = i;

			{
				RGF_PROFILE_SCOPE("Renderer2D::Init::Setting-Shader");

				SceneData.BatchRendererShader = Shader::Create();
				SceneData.BatchRendererShader->LoadFromFile("assets/Shaders/BatchRenderingShader.shader");
				SceneData.BatchRendererShader->Bind();
				SceneData.BatchRendererShader->SetUniform1iArray("u_Textures", SceneData.MaxTextureSlots, SceneData.Samplers);
				SceneData.BatchRendererShader->Unbind();
			}


			SceneData.AllTextureSlots[0] = SceneData.DefaultWhiteTexture;

		}

		float gridVerticies[] = {
			 -10000.0f, -10000.0f, 0.0f, 0.0f, 0.0f,
			  10000.0f, -10000.0f, 0.0f, 1.0f, 0.0f,
			  10000.0f,  10000.0f, 0.0f, 1.0f, 1.0f,
			 -10000.0f,  10000.0f, 0.0f, 0.0f, 1.0f

		};
		SceneData.SceneGridVertexArray = VertexArray::Create();
		SceneData.SceneGridVertexBuffer = VertexBuffer::Create(gridVerticies,  sizeof(gridVerticies));
		BufferLayout layout = {
			{BufferLayoutTypes::Float3, "aPos"},
			{BufferLayoutTypes::Float2, "aUV" }
		};


		SceneData.SceneGridVertexBuffer->SetLayout(layout);
		SceneData.SceneGridVertexArray->PushVertexBuffer(SceneData.SceneGridVertexBuffer);


		unsigned int gridIndicies[] = {
			0, 1, 2,
			2, 3, 0
		};
		Ref<IndexBuffer> aaa = IndexBuffer::Create(gridIndicies, 6);
		SceneData.SceneGridVertexArray->PushIndexBuffer(aaa);

		SceneData.SceneGridShader = Shader::Create();
		SceneData.SceneGridShader->LoadFromFile("assets/Shaders/SceneGrid.shader");
		
	}

	void Renderer2D::ShutDown() {

	}

	void Renderer2D::BeginScene(RGF::OrthographicCamera* camera) {
		RGF_PROFILE_FUNCTION();

		SceneData.m_ViewMatrix = camera->GetViewProjectionMatrix();
		SceneData.TextureSlotIndex = 1;



		SceneData.SpriteIndexCount = 0;
		SceneData.SpriteVertexDataPtr = SceneData.SpriteVertexDataBase;

		SceneData.LineIndexCount = 0;
		SceneData.LineQuadVertexData = SceneData.LineVertexDataBase;


		

	}
	void Renderer2D::EndScene() {
		RGF_PROFILE_FUNCTION();


		SceneData.BatchRendererShader->Bind();
		SceneData.BatchRendererShader->SetUniformMatrix("u_ViewProj", SceneData.m_ViewMatrix);


		unsigned int quadSize = (unsigned char*)SceneData.SpriteVertexDataPtr - (unsigned char*)SceneData.SpriteVertexDataBase;
		if (quadSize) {


			SceneData.SpriteVertexArray->Bind();
			SceneData.SpriteVertexArray->GetIbos().Bind();
			SceneData.SpriteVertexBuffer->SetData(SceneData.SpriteVertexDataBase, quadSize);
			for (unsigned int i = 0; i < SceneData.TextureSlotIndex; i++) {
				SceneData.AllTextureSlots[i]->Bind(i);
			}
			RenderCommand::DrawElements(SceneData.SpriteVertexArray, PimitiveRenderType::Triangles, SceneData.SpriteIndexCount);
			SceneData.m_Statistics.DrawCalls += 1;
		}

		unsigned int lineSize = (unsigned char*)SceneData.LineQuadVertexData - (unsigned char*)SceneData.LineVertexDataBase;
		if (lineSize) {
			SceneData.LineVertexArray->Bind();
			SceneData.LineVertexArray->GetIbos().Bind();
			SceneData.LineVertexBuffer->SetData(SceneData.LineVertexDataBase, lineSize);
			SceneData.AllTextureSlots[0]->Bind(0);
			RenderCommand::SetLineThickness(4.0f);

			RenderCommand::DrawElements(SceneData.LineVertexArray, PimitiveRenderType::Lines, SceneData.LineIndexCount);
			SceneData.m_Statistics.DrawCalls += 1;

		}
		SceneData.m_Statistics.VertexSize = quadSize + lineSize;

	}


	void Renderer2D::DrawVerticies(const std::array<glm::vec4, 4>& vertices, int vertexCount, const glm::vec4& color) {

		if (SceneData.SpriteIndexCount >= SceneData.MaxIndiciesSize) {
			BeginNewQuadBatch();
		}

		unsigned char r = color.x * 255.0f;
		unsigned char g = color.y * 255.0f;
		unsigned char b = color.z * 255.0f;
		unsigned char a = color.w * 255.0f;


		unsigned int col = a << 24 | b << 16 | g << 8 | r;

		// Vertex order = bottom left -> bottom right -> top right -> top left
		for (unsigned int i = 0; i < vertexCount; i++) {

			SceneData.SpriteVertexDataPtr->Verticies = vertices[i];
			SceneData.SpriteVertexDataPtr->Color = col;
			SceneData.SpriteVertexDataPtr->TextureCoords = SceneData.TextureCoords[0];
			SceneData.SpriteVertexDataPtr->TextureIndex = 0.0f;
			SceneData.SpriteVertexDataPtr++;
		}


		SceneData.SpriteIndexCount += 6;
		SceneData.m_Statistics.IndexCount += 6;


		

	}


	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color) {
		RGF_PROFILE_FUNCTION();
		DrawSprite(position, 0.0f, size, color);
	}


	void Renderer2D::DrawSprite(const glm::vec3& position, float rotation, const glm::vec3& size, const glm::vec4& color) {
		RGF_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), size);

		DrawSprite(transform, color);

	}


	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec3& size, const Ref<Texture>& texture, const glm::vec4& tintColor) {
		RGF_PROFILE_FUNCTION();
		DrawSprite(position, 0.0f, size, texture, tintColor);
	}



	void Renderer2D::DrawSprite(const glm::vec3& position, float rotation, const glm::vec3& size, const Ref<Texture>& texture, const glm::vec4& tintColor) {
		RGF_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), size);

		DrawSprite(transform, texture, tintColor);
	}

	

	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec3& size, const Ref<TextureBounds>& textureBounds, const glm::vec4& tintColor) {
		DrawSprite(position, 0.0f, size, textureBounds, tintColor);
	}


	void Renderer2D::DrawSprite(const glm::vec3& position, float rotation, const glm::vec3& size, const Ref<TextureBounds>& textureBounds, const glm::vec4& tintColor /*= { 1.0f, 1.0f, 1.0f, 1.0f }*/)
	{
		RGF_PROFILE_FUNCTION();


		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), size);
		DrawSprite(transform, textureBounds, tintColor);
		
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const glm::vec4& tintColor) {
		RGF_PROFILE_FUNCTION();

		constexpr unsigned int VertexCount = 4;


		unsigned char r = tintColor.r * 255.0f;
		unsigned char g = tintColor.g * 255.0f;
		unsigned char b = tintColor.b * 255.0f;
		unsigned char a = tintColor.a * 255.0f;

		unsigned int color = a << 24 | b << 16 | g << 8 | r;

		if (SceneData.SpriteIndexCount >= SceneData.MaxIndiciesSize) {
			BeginNewQuadBatch();
		}


		// Vertex order = bottom left -> bottom right -> top right -> top left
		for (unsigned int i = 0; i < VertexCount; i++) {

			SceneData.SpriteVertexDataPtr->Verticies = transform * SceneData.QuadPivotPointPositions[i];
			SceneData.SpriteVertexDataPtr->Color = color;
			SceneData.SpriteVertexDataPtr->TextureCoords = SceneData.TextureCoords[i];
			SceneData.SpriteVertexDataPtr->TextureIndex = 0.0f;
			SceneData.SpriteVertexDataPtr++;
		}


		SceneData.SpriteIndexCount += 6;
		SceneData.m_Statistics.IndexCount += 6;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const Ref<TextureBounds>& textureBounds, const glm::vec4& tintColor) {
		constexpr unsigned int VertexCount = 4;


		unsigned char r = tintColor.r * 255.0f;
		unsigned char g = tintColor.g * 255.0f;
		unsigned char b = tintColor.b * 255.0f;
		unsigned char a = tintColor.a * 255.0f;

		unsigned int color = a << 24 | b << 16 | g << 8 | r;

		if (SceneData.SpriteIndexCount >= SceneData.MaxIndiciesSize) {
			BeginNewQuadBatch();
		}


		float textureIndex = 0.0f;

		for (unsigned int i = 1; i < SceneData.TextureSlotIndex; i++) {
			if (*SceneData.AllTextureSlots[i].get() == *textureBounds->GetTexture().get()) {
				textureIndex = i;
				break;
			}
		}

		if (textureIndex == 0.0f) {
			if (SceneData.TextureSlotIndex >= SceneData.MaxTextureSlots)
				BeginNewQuadBatch();
			textureIndex = SceneData.TextureSlotIndex;
			SceneData.AllTextureSlots[SceneData.TextureSlotIndex] = textureBounds->GetTexture();
			SceneData.TextureSlotIndex++;
		}


		// Vertex order = bottom left -> bottom right -> top right -> top left
		for (unsigned int i = 0; i < VertexCount; i++) {

			SceneData.SpriteVertexDataPtr->Verticies = transform * SceneData.QuadPivotPointPositions[i];
			SceneData.SpriteVertexDataPtr->Color = color;
			SceneData.SpriteVertexDataPtr->TextureCoords = textureBounds->GetBoundsNormilized()[i];
			SceneData.SpriteVertexDataPtr->TextureIndex = textureIndex;
			SceneData.SpriteVertexDataPtr++;
		}


		SceneData.SpriteIndexCount += 6;
		SceneData.m_Statistics.IndexCount += 6;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor) {

		constexpr unsigned int VertexCount = 4;


		unsigned char r = tintColor.r * 255.0f;
		unsigned char g = tintColor.g * 255.0f;
		unsigned char b = tintColor.b * 255.0f;
		unsigned char a = tintColor.a * 255.0f;

		unsigned int color = a << 24 | b << 16 | g << 8 | r;

		if (SceneData.SpriteIndexCount >= SceneData.MaxIndiciesSize) {
			BeginNewQuadBatch();
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
				BeginNewQuadBatch();
			textureIndex = SceneData.TextureSlotIndex;
			SceneData.AllTextureSlots[SceneData.TextureSlotIndex] = texture;
			SceneData.TextureSlotIndex++;
		}


		// Vertex order = bottom left -> bottom right -> top right -> top left
		for (unsigned int i = 0; i < VertexCount; i++) {

			SceneData.SpriteVertexDataPtr->Verticies = transform * SceneData.QuadPivotPointPositions[i];
			SceneData.SpriteVertexDataPtr->Color = color;
			SceneData.SpriteVertexDataPtr->TextureCoords = SceneData.TextureCoords[i];
			SceneData.SpriteVertexDataPtr->TextureIndex = textureIndex;
			SceneData.SpriteVertexDataPtr++;
		}


		SceneData.SpriteIndexCount += 6;
		SceneData.m_Statistics.IndexCount += 6;
	}

	void Renderer2D::DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color)
	{
		RGF_PROFILE_FUNCTION();


		if (SceneData.LineIndexCount >= SceneData.MaxLineIndicesSize)
			BeginNewLineBatch();

		unsigned char r = color.r * 255.0f;
		unsigned char g = color.g * 255.0f;
		unsigned char b = color.b * 255.0f;
		unsigned char a = color.a * 255.0f;

		unsigned int c = a << 24 | b << 16 | g << 8 | r;


		SceneData.LineQuadVertexData->Verticies = p1;
		SceneData.LineQuadVertexData->Color = c;
		SceneData.LineQuadVertexData->TextureIndex = 0.0f;
		SceneData.LineQuadVertexData++;

		SceneData.LineQuadVertexData->Verticies = p2;
		SceneData.LineQuadVertexData->Color = c;
		SceneData.LineQuadVertexData->TextureIndex = 0.0f;
		SceneData.LineQuadVertexData++;


		SceneData.LineIndexCount += 2;
		SceneData.m_Statistics.IndexCount += 2;

	}

	void Renderer2D::DrawAABB(const AABB& aabb, const glm::vec4& color)
	{
		glm::vec3 corners[4] =
		{
			glm::vec3 { aabb.Min.x, aabb.Min.y, aabb.Max.z},
			glm::vec3 { aabb.Min.x, aabb.Max.y, aabb.Max.z},
			glm::vec3 { aabb.Max.x, aabb.Max.y, aabb.Max.z},
			glm::vec3 { aabb.Max.x, aabb.Min.y, aabb.Max.z},

		};

		for (uint32_t i = 0; i < 4; i++)
			Renderer2D::DrawLine(corners[i], corners[(i + 1) % 4], color);

	}

	void Renderer2D::SetBoundingBox(bool draw) {
		SceneData.DrawBoundingBoxes = draw;
	}
	const bool& Renderer2D::ShouldDrawBoundingBox() {
		return SceneData.DrawBoundingBoxes;
	}

	void Renderer2D::DrawSceneGrid(float gridAlpha) {

		SceneData.SceneGridShader->Bind();
		SceneData.SceneGridShader->SetUniformMatrix("u_ViewProj", SceneData.m_ViewMatrix);
		SceneData.SceneGridShader->SetUniform1f("u_SceneCameraZoom", gridAlpha);

		SceneData.SceneGridVertexArray->Bind();
		SceneData.SceneGridVertexArray->GetIbos().Bind();

		RenderCommand::DrawElements(SceneData.SceneGridVertexArray, PimitiveRenderType::Triangles, SceneData.SceneGridVertexArray->GetIbos().GetCount());

		SceneData.SceneGridShader->Unbind();
		SceneData.SceneGridVertexArray->Unbind();
		SceneData.SceneGridVertexArray->GetIbos().Unbind();


		

	}


	void Renderer2D::BeginNewQuadBatch() {
		RGF_PROFILE_FUNCTION();

		unsigned int size = (unsigned char*)SceneData.SpriteVertexDataPtr - (unsigned char*)SceneData.SpriteVertexDataBase;
		SceneData.SpriteVertexBuffer->SetData(SceneData.SpriteVertexDataBase, size);

		for (unsigned int i = 0; i < SceneData.TextureSlotIndex; i++) {
			SceneData.AllTextureSlots[i]->Bind(i);
		}

		SceneData.SpriteVertexArray->Bind();
		RenderCommand::DrawElements(SceneData.SpriteVertexArray, PimitiveRenderType::Triangles, SceneData.SpriteIndexCount);


		SceneData.SpriteIndexCount = 0;
		SceneData.SpriteVertexDataPtr = SceneData.SpriteVertexDataBase;
		SceneData.TextureSlotIndex = 1;
	}

	void Renderer2D::BeginNewLineBatch() {
		RGF_PROFILE_FUNCTION();

		unsigned int size = (unsigned char*)SceneData.LineQuadVertexData - (unsigned char*)SceneData.LineVertexDataBase;
		SceneData.SpriteVertexBuffer->SetData(SceneData.LineVertexDataBase, size);

		SceneData.AllTextureSlots[0]->Bind(0);
		SceneData.LineVertexArray->Bind();
		RenderCommand::DrawElements(SceneData.LineVertexArray, PimitiveRenderType::Lines, SceneData.LineIndexCount);


		SceneData.LineIndexCount = 0;
		SceneData.LineQuadVertexData = SceneData.LineVertexDataBase;
		SceneData.TextureSlotIndex = 1;
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