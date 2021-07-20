#include "PXpch.h"
#include "Renderer2D.h"


#include "Pixelate/Rendering/RendererAPI.h"
#include "Pixelate/Rendering/RenderCommand.h"

#include "Pixelate/Debug/Instrumentor.h"
#include "Pixelate/Rendering/TextureManager.h"

#include "Pixelate/Rendering/RendererCapabilities.h"
#include "API/FrameBuffer.h"




namespace Pixelate {

	// Data that will be sent to the GPU.
	struct PX_API SpriteVertexData {
		glm::vec3 Verticies;
		unsigned int Color;
		glm::vec2 TextureCoords;
		float TextureIndex;
		int EntityID;
	};


	struct PX_API LineVertexData {
		glm::vec3 Verticies;
		unsigned int Color;
		glm::vec2 TextureCoords;
		float TextureIndex;
		int EntityID;
	};

	struct Renderer2DData {

		static const unsigned int MaxSprites = 20000;
		static const unsigned int MaxVerticiesSize = MaxSprites * 4;
		static const unsigned int MaxIndiciesSize = MaxSprites * 6;

		static const unsigned int MaxLines = 10000;
		static const unsigned int MaxLineVerticesSize = MaxLines * 2;
		static const unsigned int MaxLineIndicesSize = MaxLines * 6;



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
		Renderer2D::RenderingStatistics m_Statistics;


		glm::mat4 m_ViewMatrix;

		bool DrawBoundingBoxes = false;
	};

	static Renderer2DData* SceneData = nullptr;

	int RendererCapabilities::MaxTextureSlots = 0; // This may need to be moved into its own translation unit potentially in the future


	void Renderer2D::Init() {
		PX_PROFILE_FUNCTION();
		SceneData = new Renderer2DData;
		FramebufferPool::Init();
		{
			PX_PROFILE_SCOPE("Renderer2D::Init::Setting-VertexBuffer");

			SceneData->m_Statistics.MaxIndexBuferSize = SceneData->MaxIndiciesSize;
			SceneData->m_Statistics.MaxVertexBufferSize = SceneData->MaxVerticiesSize;
			SceneData->m_Statistics.MaxSprites = SceneData->MaxSprites;

			SceneData->SpriteVertexArray = VertexArray::Create();
			SceneData->SpriteVertexBuffer = VertexBuffer::Create(SceneData->MaxVerticiesSize * sizeof(SpriteVertexData));
		

			BufferLayout layout = 
			{
				{ BufferLayoutTypes::Float3, "aPos"},
				{ BufferLayoutTypes::UChar4, "aColor", true},
				{ BufferLayoutTypes::Float2, "aTexCoords"},
				{ BufferLayoutTypes::Float, "aTexIndex"},
				{ BufferLayoutTypes::Int, "aEntityID" }

			};

			SceneData->SpriteVertexBuffer->SetLayout(layout);
			SceneData->SpriteVertexArray->PushVertexBuffer(SceneData->SpriteVertexBuffer);

			SceneData->SpriteVertexDataBase = new SpriteVertexData[SceneData->MaxVerticiesSize];

			// For lines
			SceneData->LineVertexArray = VertexArray::Create();
			SceneData->LineVertexBuffer = VertexBuffer::Create(SceneData->MaxLineVerticesSize * sizeof(LineVertexData));


			SceneData->LineVertexBuffer->SetLayout(layout);
			SceneData->LineVertexArray->PushVertexBuffer(SceneData->LineVertexBuffer);


			SceneData->LineVertexDataBase = new LineVertexData[SceneData->MaxLineVerticesSize];

		}

		{

			PX_PROFILE_SCOPE("Renderer2D::Init::Setting-IndexBuffer");
			unsigned int* indices = new unsigned int[SceneData->MaxIndiciesSize];

			int offset = 0;
			for (unsigned int i = 0; i < SceneData->MaxIndiciesSize; i += 6) {
				indices[i] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;
				indices[i + 3] = offset + 2;

				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}

			Ref<IndexBuffer> ibo = IndexBuffer::Create(indices, SceneData->MaxIndiciesSize);
			SceneData->SpriteVertexArray->PushIndexBuffer(ibo);
			SceneData->SpriteVertexArray->Unbind();
			delete[] indices;

			// for rendering lines
			unsigned int* lineIndices = new unsigned int[SceneData->MaxLineIndicesSize];
			for (unsigned int i = 0; i < SceneData->MaxLineIndicesSize; i++) {
				lineIndices[i] = i;
			}

			Ref<IndexBuffer> lineIbo = IndexBuffer::Create(lineIndices, SceneData->MaxLineIndicesSize);
			SceneData->LineVertexArray->PushIndexBuffer(lineIbo);

			SceneData->LineVertexArray->Unbind();
			delete[] lineIndices;
		}
		{

			PX_PROFILE_SCOPE("Renderer2D::Init::Setting-Texture");

			TextureManager::Init();


			SceneData->QuadPivotPointPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f }; // -- bottom left
			SceneData->QuadPivotPointPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f}; // -- bottom right
			SceneData->QuadPivotPointPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f}; // -- top right
			SceneData->QuadPivotPointPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f}; // -- top left


			
			int* samplers = new int[RendererCapabilities::MaxTextureSlots];
			for (unsigned int i = 0; i < RendererCapabilities::MaxTextureSlots; i++)
				samplers[i] = i;

			{
				PX_PROFILE_SCOPE("Renderer2D::Init::Setting-Shader");

				SceneData->BatchRendererShader = Shader::Create();
				SceneData->BatchRendererShader->LoadFromFile("assets/Shaders/BatchRenderingShader.shader");
				SceneData->BatchRendererShader->Bind();
				SceneData->BatchRendererShader->SetUniform1iArray("u_Textures", RendererCapabilities::MaxTextureSlots, samplers);
				SceneData->BatchRendererShader->Unbind();

			}
			delete[] samplers;

		}

		float gridVerticies[] = {
			 -10000.0f, -10000.0f, 0.0f, 0.0f, 0.0f,
			  10000.0f, -10000.0f, 0.0f, 1.0f, 0.0f,
			  10000.0f,  10000.0f, 0.0f, 1.0f, 1.0f,
			 -10000.0f,  10000.0f, 0.0f, 0.0f, 1.0f

		};
		SceneData->SceneGridVertexArray = VertexArray::Create();
		SceneData->SceneGridVertexBuffer = VertexBuffer::Create(gridVerticies,  sizeof(gridVerticies));
		BufferLayout layout = {
			{BufferLayoutTypes::Float3, "aPos"},
			{BufferLayoutTypes::Float2, "aUV" }
		};


		SceneData->SceneGridVertexBuffer->SetLayout(layout);
		SceneData->SceneGridVertexArray->PushVertexBuffer(SceneData->SceneGridVertexBuffer);


		unsigned int gridIndicies[] = {
			0, 1, 2,
			2, 3, 0
		};
		Ref<IndexBuffer> aaa = IndexBuffer::Create(gridIndicies, 6);
		SceneData->SceneGridVertexArray->PushIndexBuffer(aaa);

		SceneData->SceneGridShader = Shader::Create();
		SceneData->SceneGridShader->LoadFromFile("assets/Shaders/SceneGrid.shader");
		
	}

	void Renderer2D::ShutDown() {
		TextureManager::Shutdown();
		FramebufferPool::Destroy();


		delete SceneData;
	}

	void Renderer2D::BeginScene(Pixelate::OrthographicCamera* camera) {
		PX_PROFILE_FUNCTION();

		SceneData->m_ViewMatrix = camera->GetViewProjectionMatrix();
		TextureManager::GetManagerData().TextureSlotIndex = 1;


		SceneData->SpriteIndexCount = 0;
		SceneData->SpriteVertexDataPtr = SceneData->SpriteVertexDataBase;

		SceneData->LineIndexCount = 0;
		SceneData->LineQuadVertexData = SceneData->LineVertexDataBase;


		

	}
	void Renderer2D::EndScene() {
		PX_PROFILE_FUNCTION();


		SceneData->BatchRendererShader->Bind();
		SceneData->BatchRendererShader->SetUniformMatrix("u_ViewProj", SceneData->m_ViewMatrix);


		unsigned int quadSize = (unsigned char*)SceneData->SpriteVertexDataPtr - (unsigned char*)SceneData->SpriteVertexDataBase;
		if (quadSize) {


			SceneData->SpriteVertexArray->Bind();
			SceneData->SpriteVertexArray->GetIbos().Bind();
			SceneData->SpriteVertexBuffer->SetData(SceneData->SpriteVertexDataBase, quadSize);


			TextureManager::BindAllTextures();

			RenderCommand::DrawElements(SceneData->SpriteVertexArray, PimitiveRenderType::Triangles, SceneData->SpriteIndexCount);
			SceneData->m_Statistics.DrawCalls += 1;
		}

		unsigned int lineSize = (unsigned char*)SceneData->LineQuadVertexData - (unsigned char*)SceneData->LineVertexDataBase;
		if (lineSize) {
			SceneData->LineVertexArray->Bind();
			SceneData->LineVertexArray->GetIbos().Bind();
			SceneData->LineVertexBuffer->SetData(SceneData->LineVertexDataBase, lineSize);
			TextureManager::GetDefaultTexture()->Bind();
			RenderCommand::SetLineThickness(1.0f);

			RenderCommand::DrawElements(SceneData->LineVertexArray, PimitiveRenderType::Lines, SceneData->LineIndexCount);
			SceneData->m_Statistics.DrawCalls += 1;

		}
		SceneData->m_Statistics.VertexSize = quadSize + lineSize;

	}


	void Renderer2D::DrawVerticies(const std::array<glm::vec4, 4>& vertices, int vertexCount, const glm::vec4& color) {

		if (SceneData->SpriteIndexCount >= SceneData->MaxIndiciesSize) {
			BeginNewQuadBatch();
		}

		unsigned char r = color.x * 255.0f;
		unsigned char g = color.y * 255.0f;
		unsigned char b = color.z * 255.0f;
		unsigned char a = color.w * 255.0f;


		unsigned int col = a << 24 | b << 16 | g << 8 | r;

		// Vertex order = bottom left -> bottom right -> top right -> top left
		for (unsigned int i = 0; i < vertexCount; i++) {

			SceneData->SpriteVertexDataPtr->Verticies = vertices[i];
			SceneData->SpriteVertexDataPtr->Color = col;
			SceneData->SpriteVertexDataPtr->TextureCoords = {0.0f, 0.0f};
			SceneData->SpriteVertexDataPtr->TextureIndex = 0.0f;
			SceneData->SpriteVertexDataPtr++;
		}


		SceneData->SpriteIndexCount += 6;
		SceneData->m_Statistics.IndexCount += 6;


		

	}


	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, int entityID) {
		PX_PROFILE_FUNCTION();
		DrawSprite(position, 0.0f, size, color, entityID);
	}


	void Renderer2D::DrawSprite(const glm::vec3& position, float rotation, const glm::vec3& size, const glm::vec4& color, int entityID) {
		PX_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), size);

		DrawSprite(transform, color, entityID);

	}


	void Renderer2D::DrawSprite(const glm::vec3& position, const glm::vec3& size, const Ref<Texture>& texture, const glm::vec4& tintColor, int entityID) {
		PX_PROFILE_FUNCTION();
		DrawSprite(position, 0.0f, size, texture, tintColor, entityID);
	}



	void Renderer2D::DrawSprite(const glm::vec3& position, float rotation, const glm::vec3& size, const Ref<Texture>& texture, const glm::vec4& tintColor, int entityID) {
		PX_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), size);

		DrawSprite(transform, texture, tintColor, entityID);
	}

	


	void Renderer2D::DrawSprite(const glm::mat4& transform, const glm::vec4& tintColor, int entityID) {
		PX_PROFILE_FUNCTION();

		constexpr unsigned int VertexCount = 4;


		unsigned char r = tintColor.r * 255.0f;
		unsigned char g = tintColor.g * 255.0f;
		unsigned char b = tintColor.b * 255.0f;
		unsigned char a = tintColor.a * 255.0f;

		unsigned int color = a << 24 | b << 16 | g << 8 | r;

		if (SceneData->SpriteIndexCount >= SceneData->MaxIndiciesSize) {
			BeginNewQuadBatch();
		}


		// Vertex order = bottom left -> bottom right -> top right -> top left
		for (unsigned int i = 0; i < VertexCount; i++) {

			SceneData->SpriteVertexDataPtr->Verticies = transform * SceneData->QuadPivotPointPositions[i];
			SceneData->SpriteVertexDataPtr->Color = color;
			SceneData->SpriteVertexDataPtr->TextureCoords = TextureManager::GetManagerData().TextureCoords[i];
			SceneData->SpriteVertexDataPtr->TextureIndex = 0.0f;
			SceneData->SpriteVertexDataPtr->EntityID = entityID;
			SceneData->SpriteVertexDataPtr++;
		}


		SceneData->SpriteIndexCount += 6;
		SceneData->m_Statistics.IndexCount += 6;
	}


	void Renderer2D::DrawSprite(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor, int entityID) {

		constexpr unsigned int VertexCount = 4;


		unsigned char r = tintColor.r * 255.0f;
		unsigned char g = tintColor.g * 255.0f;
		unsigned char b = tintColor.b * 255.0f;
		unsigned char a = tintColor.a * 255.0f;

		unsigned int color = a << 24 | b << 16 | g << 8 | r;

		if (SceneData->SpriteIndexCount >= SceneData->MaxIndiciesSize) {
			BeginNewQuadBatch();
		}

		float textureIndex = TextureManager::IsTextureValid(texture);

		if (textureIndex == 0.0f) {
			auto& manager = TextureManager::GetManagerData();
			if (manager.TextureSlotIndex >= RendererCapabilities::MaxTextureSlots)
				BeginNewQuadBatch();
			textureIndex = manager.TextureSlotIndex;
			TextureManager::DirectAdd(texture);
		}


		// Vertex order = bottom left -> bottom right -> top right -> top left
		for (unsigned int i = 0; i < VertexCount; i++) {

			SceneData->SpriteVertexDataPtr->Verticies = transform * SceneData->QuadPivotPointPositions[i];
			SceneData->SpriteVertexDataPtr->Color = color;
			SceneData->SpriteVertexDataPtr->TextureCoords = TextureManager::GetManagerData().TextureCoords[i];
			SceneData->SpriteVertexDataPtr->TextureIndex = textureIndex;
			SceneData->SpriteVertexDataPtr->EntityID = entityID;
			SceneData->SpriteVertexDataPtr++;
		}


		SceneData->SpriteIndexCount += 6;
		SceneData->m_Statistics.IndexCount += 6;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const Ref<Texture>& texture, const Rect& rect, const glm::vec4& tintColor, int entityID) {
		constexpr unsigned int VertexCount = 4;


		unsigned char r = tintColor.r * 255.0f;
		unsigned char g = tintColor.g * 255.0f;
		unsigned char b = tintColor.b * 255.0f;
		unsigned char a = tintColor.a * 255.0f;

		unsigned int color = a << 24 | b << 16 | g << 8 | r;

		if (SceneData->SpriteIndexCount >= SceneData->MaxIndiciesSize) {
			BeginNewQuadBatch();
		}

		float textureIndex = TextureManager::IsTextureValid(texture);

		if (textureIndex == 0.0f) {
			auto& manager = TextureManager::GetManagerData();
			if (manager.TextureSlotIndex >= RendererCapabilities::MaxTextureSlots)
				BeginNewQuadBatch();
			textureIndex = manager.TextureSlotIndex;
			TextureManager::DirectAdd(texture);
		}

		const auto& texCoords = NormalizedCoordinates(rect, texture);
		// Vertex order = bottom left -> bottom right -> top right -> top left
		for (unsigned int i = 0; i < VertexCount; i++) {

			SceneData->SpriteVertexDataPtr->Verticies = transform * SceneData->QuadPivotPointPositions[i];
			SceneData->SpriteVertexDataPtr->Color = color;
			SceneData->SpriteVertexDataPtr->TextureCoords = texCoords[i];
			SceneData->SpriteVertexDataPtr->TextureIndex = textureIndex;
			SceneData->SpriteVertexDataPtr->EntityID = entityID;
			SceneData->SpriteVertexDataPtr++;
		}


		SceneData->SpriteIndexCount += 6;
		SceneData->m_Statistics.IndexCount += 6;
	}

	void Renderer2D::DrawSprite(const TransformComponent& transform, const SpriteRendererComponent& sprite, int entityID)
	{
		PX_PROFILE_FUNCTION();
		DrawSprite(transform.Transform, sprite.Texture, sprite.Rect, sprite.TintColor, entityID);
	}

	void Renderer2D::DrawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color)
	{
		PX_PROFILE_FUNCTION();


		if (SceneData->LineIndexCount >= SceneData->MaxLineIndicesSize)
			BeginNewLineBatch();

		unsigned char r = color.r * 255.0f;
		unsigned char g = color.g * 255.0f;
		unsigned char b = color.b * 255.0f;
		unsigned char a = color.a * 255.0f;

		unsigned int c = a << 24 | b << 16 | g << 8 | r;


		SceneData->LineQuadVertexData->Verticies = p1;
		SceneData->LineQuadVertexData->Color = c;
		SceneData->LineQuadVertexData->TextureIndex = 0.0f;
		SceneData->LineQuadVertexData->EntityID = -1;
		SceneData->LineQuadVertexData++;

		SceneData->LineQuadVertexData->Verticies = p2;
		SceneData->LineQuadVertexData->Color = c;
		SceneData->LineQuadVertexData->TextureIndex = 0.0f;
		SceneData->LineQuadVertexData->EntityID = -1;
		SceneData->LineQuadVertexData++;


		SceneData->LineIndexCount += 2;
		SceneData->m_Statistics.IndexCount += 2;

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
		SceneData->DrawBoundingBoxes = draw;
	}
	const bool& Renderer2D::ShouldDrawBoundingBox() {
		return SceneData->DrawBoundingBoxes;
	}

	void Renderer2D::DrawSceneGrid(float gridAlpha) {

		SceneData->SceneGridShader->Bind();
		SceneData->SceneGridShader->SetUniformMatrix("u_ViewProj", SceneData->m_ViewMatrix);
		SceneData->SceneGridShader->SetUniform1f("u_SceneCameraZoom", gridAlpha);

		SceneData->SceneGridVertexArray->Bind();
		SceneData->SceneGridVertexArray->GetIbos().Bind();

		RenderCommand::DrawElements(SceneData->SceneGridVertexArray, PimitiveRenderType::Triangles, SceneData->SceneGridVertexArray->GetIbos().GetCount());

		SceneData->SceneGridShader->Unbind();
		SceneData->SceneGridVertexArray->Unbind();
		SceneData->SceneGridVertexArray->GetIbos().Unbind();


		

	}


	void Renderer2D::BeginNewQuadBatch() {
		PX_PROFILE_FUNCTION();

		unsigned int size = (unsigned char*)SceneData->SpriteVertexDataPtr - (unsigned char*)SceneData->SpriteVertexDataBase;
		SceneData->SpriteVertexBuffer->SetData(SceneData->SpriteVertexDataBase, size);

		TextureManager::BindAllTextures();

		SceneData->SpriteVertexArray->Bind();
		RenderCommand::DrawElements(SceneData->SpriteVertexArray, PimitiveRenderType::Triangles, SceneData->SpriteIndexCount);


		SceneData->SpriteIndexCount = 0;
		SceneData->SpriteVertexDataPtr = SceneData->SpriteVertexDataBase;
		TextureManager::GetManagerData().TextureSlotIndex = 1;

	}

	void Renderer2D::BeginNewLineBatch() {
		PX_PROFILE_FUNCTION();

		unsigned int size = (unsigned char*)SceneData->LineQuadVertexData - (unsigned char*)SceneData->LineVertexDataBase;
		SceneData->SpriteVertexBuffer->SetData(SceneData->LineVertexDataBase, size);

		TextureManager::GetDefaultTexture()->Bind();

		SceneData->LineVertexArray->Bind();
		RenderCommand::DrawElements(SceneData->LineVertexArray, PimitiveRenderType::Lines, SceneData->LineIndexCount);


		SceneData->LineIndexCount = 0;
		SceneData->LineQuadVertexData = SceneData->LineVertexDataBase;

		TextureManager::GetManagerData().TextureSlotIndex = 1;

	}

	std::array<glm::vec2, 4> Renderer2D::NormalizedCoordinates(const Rect& rect, const Ref<Texture>& texture) {
		std::array<glm::vec2, 4> result;

		float xOffset = (float)rect.Position.x / texture->GetWidth();
		float yOffset = (float)rect.Position.y / texture->GetHeight();

		float xSize = (float)rect.Scale.x / texture->GetWidth();
		float ySize = (float)rect.Scale.y / texture->GetHeight();



		result[0] = { xOffset, 1.0f - (yOffset + ySize) }; // bottom left
		result[1] = { xOffset + xSize, 1.0f - (yOffset + ySize) }; // bottom right
		result[2] = { xOffset + xSize, 1.0f - yOffset }; // top right
		result[3] = { xOffset, 1.0f - yOffset }; // top left

		return result;

	}

	void Renderer2D::ResetStatistics() {
		PX_PROFILE_FUNCTION();
		SceneData->m_Statistics.DrawCalls = 0;
		SceneData->m_Statistics.IndexCount = 0;
		SceneData->m_Statistics.VertexSize = 0;
	}

	Pixelate::Renderer2D::RenderingStatistics& Renderer2D::GetStats() {
		return SceneData->m_Statistics;
	}

	
}