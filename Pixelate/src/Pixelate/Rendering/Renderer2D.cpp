#include "PXpch.h"
#include "Renderer2D.h"


#ifdef _DEBUG
#include "Pixelate/Rendering/RendererAPI.h"
#include "Pixelate/Rendering/RenderCommand.h"
#endif // _DEBUG

#include "Pixelate/Debug/Instrumentor.h"
#include "Pixelate/Rendering/TextureManager.h"

#include "Pixelate/Rendering/RendererCapabilities.h"
#include "API/FrameBuffer.h"

#include <math.h>
#include <Pixelate\Core\Timer.h>

#include "Pixelate/Scene/Components.h"


namespace Pixelate {

	

	
	// redundant 
	struct PX_API LineVertexData {
		glm::vec3 Verticies;
		unsigned int Color;
		glm::vec2 TextureCoords;
		float TextureIndex;
		int EntityID;
	};

	struct CameraData
	{
		glm::mat4 ViewProj;
	};

	struct GridData
	{
		glm::mat4 ViewProj;
		float Zoom;
	};

	struct Renderer2DData {

		static const unsigned int MaxLines = 10000;
		static const unsigned int MaxLineVerticesSize = MaxLines * 2;
		static const unsigned int MaxLineIndicesSize = MaxLines * 6;


		std::unordered_map<Ref<Shader>, Ref<DrawData>> DrawList;
		Ref<UniformBuffer> CameraUniformBufferData;
		CameraData CameraBuffer;



		// For rendering lines
		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		unsigned int LineIndexCount = 0;

		LineVertexData* LineVertexDataBase = nullptr;
		LineVertexData* LineQuadVertexData = nullptr;


		// For rendering line strips
		Ref<VertexArray> LineStripVertexArray;
		Ref<VertexBuffer> LineStripVertexBuffer;
		uint32_t LineStripIndexCount = 0;
		uint32_t LineStripDrawCalls = 0;

		LineVertexData* LineStripVertexDataBase = nullptr;
		LineVertexData* LineStripQuadVertexData = nullptr;


		// For drawing the scene grid.
		Ref<VertexArray> SceneGridVertexArray;
		Ref<VertexBuffer> SceneGridVertexBuffer;
		Ref<UniformBuffer> GridUniformBufferData;
		GridData GridBufferData;



		std::array<glm::vec4, 4> QuadPivotPointPositions;


		glm::mat4 m_ViewMatrix;

		bool DrawBoundingBoxes = false;
	};

	static Renderer2DData* SceneData = nullptr;
	ShaderLibrary Renderer2D::s_ShaderLibrary;

	int RendererCapabilities::MaxTextureSlots = 0; // This may need to be moved into its own translation unit potentially in the future


	void Renderer2D::Init() {
		PX_PROFILE_FUNCTION();
		SceneData = new Renderer2DData;
		FramebufferPool::Init();
		{
			PX_PROFILE_SCOPE("Renderer2D::Init::Setting-VertexBuffer");

			
			// For lines

			BufferLayout layout =
			{
				{ BufferLayoutTypes::Float3, "aPos"},
				{ BufferLayoutTypes::UChar4, "aColor", true},
				{ BufferLayoutTypes::Float2, "aTexCoords"},
				{ BufferLayoutTypes::Float, "aTexIndex"},
				{ BufferLayoutTypes::Int, "aEntityID" }

			};

			SceneData->LineVertexArray = VertexArray::Create();
			SceneData->LineVertexBuffer = VertexBuffer::Create(SceneData->MaxLineVerticesSize * sizeof(LineVertexData));


			SceneData->LineVertexBuffer->SetLayout(layout);
			SceneData->LineVertexArray->PushVertexBuffer(SceneData->LineVertexBuffer);
			SceneData->LineVertexDataBase = new LineVertexData[SceneData->MaxLineVerticesSize];

			// For line strips

			SceneData->LineStripVertexArray = VertexArray::Create();
			SceneData->LineStripVertexBuffer = VertexBuffer::Create(SceneData->MaxLineVerticesSize * sizeof(LineVertexData));


			SceneData->LineStripVertexBuffer->SetLayout(layout);
			SceneData->LineStripVertexArray->PushVertexBuffer(SceneData->LineStripVertexBuffer);


			SceneData->LineStripVertexDataBase = new LineVertexData[SceneData->MaxLineVerticesSize];

		}

		{

			// for rendering lines
			unsigned int* lineIndices = new unsigned int[SceneData->MaxLineIndicesSize];
			for (unsigned int i = 0; i < SceneData->MaxLineIndicesSize; i++) {
				lineIndices[i] = i;
			}

			Ref<IndexBuffer> lineIbo = IndexBuffer::Create(lineIndices, SceneData->MaxLineIndicesSize);
			SceneData->LineVertexArray->PushIndexBuffer(lineIbo);
			SceneData->LineVertexArray->Unbind();


			SceneData->LineStripVertexArray->PushIndexBuffer(lineIbo);
			SceneData->LineStripVertexArray->Unbind();
			delete[] lineIndices;
		}
		{

			PX_PROFILE_SCOPE("Renderer2D::Init::Setting-Texture");

			TextureManager::Init();


			SceneData->QuadPivotPointPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f }; // -- bottom left
			SceneData->QuadPivotPointPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f}; // -- bottom right
			SceneData->QuadPivotPointPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f}; // -- top right
			SceneData->QuadPivotPointPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f}; // -- top left



			{
				PX_PROFILE_SCOPE("Renderer2D::Init::Setting-Shader");

				Timer timer;

				s_ShaderLibrary.Load("DefaultTexturedShader", "Shaders/DefaultTexturedShader.pxShader");
				PX_CORE_MSG("It took %f(ms) to load DefaultTexturedShader.Shader!\n", timer.GetElapsedMillis());
				SceneData->CameraUniformBufferData = UniformBuffer::Create(sizeof(CameraData), 0);

				
			}

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

		{
			Timer timer;
			s_ShaderLibrary.Load("GridShader", "Shaders/SceneGrid.pxShader");
			PX_CORE_MSG("It took %f(ms) to load SceneGrid.Shader!\n", timer.GetElapsedMillis());
			SceneData->GridUniformBufferData = UniformBuffer::Create(sizeof(GridData), 0);
		}
		
	}

	void Renderer2D::ShutDown() {
		TextureManager::Shutdown();
		FramebufferPool::Destroy();


		delete SceneData;
	}

	void Renderer2D::BeginScene(Pixelate::OrthographicCamera* camera) {
		PX_PROFILE_FUNCTION();

		SceneData->CameraBuffer.ViewProj = camera->GetViewProjectionMatrix();

		TextureManager::GetManagerData().TextureSlotIndex = 1;


		for (auto& commands : SceneData->DrawList)
		{
			commands.second->IndexCount = 0;
			commands.second->PtrData = commands.second->PtrBase;

		}
		SceneData->LineIndexCount = 0;
		SceneData->LineQuadVertexData = SceneData->LineVertexDataBase;

		SceneData->LineStripIndexCount = 0;
		SceneData->LineStripDrawCalls = 0;
		SceneData->LineStripQuadVertexData = SceneData->LineStripVertexDataBase;


		

	}
	void Renderer2D::EndScene() {
		PX_PROFILE_FUNCTION();
		
		


		SceneData->CameraUniformBufferData->SetData(&SceneData->CameraBuffer, sizeof(CameraData), 0);
		for (auto& commands : SceneData->DrawList)
		{
			commands.first->Bind();
			auto& data = commands.second;

			uint32_t size = (uint8_t*)data->PtrData - (uint8_t*)data->PtrBase;

			if (size)
			{
				data->Vao->Bind();
				data->Vao->GetIbos().Bind();
				data->Vbo->SetData(data->PtrBase, size);

				TextureManager::BindAllTextures();

				RenderCommand::DrawElements(data->Vao, PimitiveRenderType::Triangles, data->IndexCount);

			}
		}


		unsigned int lineSize = (unsigned char*)SceneData->LineQuadVertexData - (unsigned char*)SceneData->LineVertexDataBase;
		if (lineSize) {
			SceneData->LineVertexArray->Bind();
			SceneData->LineVertexArray->GetIbos().Bind();
			SceneData->LineVertexBuffer->SetData(SceneData->LineVertexDataBase, lineSize);
			TextureManager::GetDefaultTexture()->Bind();
			RenderCommand::SetLineThickness(1.0f);

			RenderCommand::DrawElements(SceneData->LineVertexArray, PimitiveRenderType::Lines, SceneData->LineIndexCount);

		}


		TextureManager::GetDefaultTexture()->Bind();
		RenderCommand::SetLineThickness(1.0f);

		for (int i = 0; i < SceneData->LineStripDrawCalls; i++) {

			uint32_t lineStripSize = (uint8_t*)SceneData->LineStripQuadVertexData - (uint8_t*)SceneData->LineStripVertexDataBase;
			uint32_t size = lineStripSize / SceneData->LineStripDrawCalls;

			LineVertexData* ptrBase = SceneData->LineStripVertexDataBase;
			uint32_t indexCount = SceneData->LineStripIndexCount / SceneData->LineStripDrawCalls;

			ptrBase += i*indexCount;

			
			if (lineStripSize) {
				SceneData->LineStripVertexArray->Bind();
				SceneData->LineStripVertexArray->GetIbos().Bind();
				SceneData->LineStripVertexBuffer->SetData(ptrBase, size);

				RenderCommand::DrawElements(SceneData->LineStripVertexArray, PimitiveRenderType::LineStrip, indexCount);

			}

		}


	}


	void Renderer2D::DrawVerticies(glm::vec4* vertices, int vertexCount, const glm::vec4& color) {

// 		if (SceneData->SpriteIndexCount >= SceneData->MaxIndiciesSize) {
// 			BeginNewQuadBatch();
// 		}
// 
// 		unsigned char r = color.x * 255.0f;
// 		unsigned char g = color.y * 255.0f;
// 		unsigned char b = color.z * 255.0f;
// 		unsigned char a = color.w * 255.0f;
// 
// 
// 		unsigned int col = a << 24 | b << 16 | g << 8 | r;
// 
// 		// Vertex order = bottom left -> bottom right -> top right -> top left
// 		for (unsigned int i = 0; i < vertexCount; i++) {
// 
// 			SceneData->SpriteVertexDataPtr->Verticies = vertices[i];
// 			SceneData->SpriteVertexDataPtr->Color = col;
// 			SceneData->SpriteVertexDataPtr->TextureCoords = {0.0f, 0.0f};
// 			SceneData->SpriteVertexDataPtr->TextureIndex = 0.0f;
// 			SceneData->SpriteVertexDataPtr->EntityID = -1;
// 			SceneData->SpriteVertexDataPtr++;
// 		}
// 
// 		if (vertexCount < 4) {
// 			glm::vec4 v = vertices[2];
// 			SceneData->SpriteVertexDataPtr->Verticies = v;
// 			SceneData->SpriteVertexDataPtr->Color = col;
// 			SceneData->SpriteVertexDataPtr->TextureCoords = { 0.0f, 0.0f };
// 			SceneData->SpriteVertexDataPtr->TextureIndex = 0.0f;
// 			SceneData->SpriteVertexDataPtr->EntityID = -1;
// 			SceneData->SpriteVertexDataPtr++;
// 		}
// 
// 
// 		SceneData->SpriteIndexCount += 6;
// 		SceneData->m_Statistics.IndexCount += 6;
// 

		

	}


	void Renderer2D::DrawSpriteWithShader(const glm::mat4& transform, const Ref<Texture>& texture, const Rect& textureDestRect, const glm::vec4& tintColor, const Ref<Shader>& shader, int entityID)
	{

		if (SceneData->DrawList.find(shader) == SceneData->DrawList.end())
		{
			SceneData->DrawList[shader] = CreateRef<DrawData>();
			SceneData->DrawList[shader]->Invalidate();
		}
		auto& command = SceneData->DrawList[shader];


		constexpr unsigned int VertexCount = 4;


		unsigned char r = tintColor.r * 255.0f;
		unsigned char g = tintColor.g * 255.0f;
		unsigned char b = tintColor.b * 255.0f;
		unsigned char a = tintColor.a * 255.0f;

		unsigned int color = a << 24 | b << 16 | g << 8 | r;

// 		if (command->IndexCount >= command->MaxIndexSize)
// 		{
// 			BeginNewQuadBatch();
// 		}

		float textureIndex = 0.0f;
		std::array<glm::vec2, 4> texCoords = { {{0.0f, 0.0f}, {0.0f, 0.0f},{0.0f, 0.0f},{0.0f, 0.0f}}};
		if (texture)
		{
			textureIndex = TextureManager::IsTextureValid(texture);

			if (textureIndex == 0.0f) {
				auto& manager = TextureManager::GetManagerData();
				if (manager.TextureSlotIndex >= RendererCapabilities::MaxTextureSlots)
					BeginNewQuadBatch();
				textureIndex = manager.TextureSlotIndex;
				TextureManager::DirectAdd(texture);
			}

			texCoords = NormalizedCoordinates(textureDestRect, texture);
		}
		// Vertex order = bottom left -> bottom right -> top right -> top left
		for (unsigned int i = 0; i < VertexCount; i++) {

			command->PtrData->Verticies = transform * SceneData->QuadPivotPointPositions[i];
			command->PtrData->Color = color;
			command->PtrData->TextureCoords = texCoords[i];
			command->PtrData->TextureIndex = textureIndex;
			command->PtrData->EntityID = entityID;
			command->PtrData++;
		}


		command->IndexCount += 6;
		//SceneData->m_Statistics.IndexCount += 6;
	}

	void Renderer2D::DrawSpriteWithShader(const TransformComponent& transform, const SpriteRendererComponent& sprite, int entityID)
	{
		DrawSpriteWithShader(transform.Transform, sprite.Texture, sprite.Rect, sprite.TintColor, sprite.Shader, entityID);

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

	void Renderer2D::DrawCircle(const glm::vec2& center, float radius, const glm::vec4& color) {

		float pi2 = 2.0f * 3.14f;
		constexpr int sides = 64;
		constexpr int verts = sides + 1;
		
		uint8_t r = color.r * 255.0f;
		uint8_t g = color.g * 255.0f;
		uint8_t b = color.b * 255.0f;
		uint8_t a = color.a * 255.0f;

		uint32_t c = a << 24 | b << 16 | g << 8 | r;


		for (uint32_t angle = 0; angle < verts; angle++) {
			glm::vec3 result;
			result.x = center.x + (radius * cos(angle * pi2 / sides));
			result.y = center.y + (radius * sin(angle * pi2 / sides));
			result.z = 0.0f;


			SceneData->LineStripQuadVertexData->Verticies = result;
			SceneData->LineStripQuadVertexData->Color = c;
			SceneData->LineStripQuadVertexData->TextureCoords = { 0.0f, 0.0f };
			SceneData->LineStripQuadVertexData->TextureIndex = 0.0f;
			SceneData->LineStripQuadVertexData->EntityID = -1;
			SceneData->LineStripQuadVertexData++;


			SceneData->LineStripIndexCount++;


		}

		SceneData->LineStripDrawCalls++;


	}

	void Renderer2D::SetBoundingBox(bool draw) {
		SceneData->DrawBoundingBoxes = draw;
	}
	const bool& Renderer2D::ShouldDrawBoundingBox() {
		return SceneData->DrawBoundingBoxes;
	}

	

	std::unordered_map<Pixelate::Ref<Pixelate::Shader>, Pixelate::Ref<Pixelate::DrawData>>& Renderer2D::GetDrawList()
	{
		return SceneData->DrawList;
	}

	void Renderer2D::DrawSceneGrid(float gridAlpha) {

		s_ShaderLibrary.Get()["GridShader"]->Bind();

		SceneData->GridBufferData.ViewProj = SceneData->CameraBuffer.ViewProj;
		SceneData->GridBufferData.Zoom = gridAlpha;
		SceneData->GridUniformBufferData->SetData(&SceneData->GridBufferData, sizeof(GridData), 0);


		SceneData->SceneGridVertexArray->Bind();
		SceneData->SceneGridVertexArray->GetIbos().Bind();

		RenderCommand::DrawElements(SceneData->SceneGridVertexArray, PimitiveRenderType::Triangles, SceneData->SceneGridVertexArray->GetIbos().GetCount());

		s_ShaderLibrary.Get()["GridShader"]->Unbind();
		SceneData->SceneGridVertexArray->Unbind();
		SceneData->SceneGridVertexArray->GetIbos().Unbind();


		

	}


	void Renderer2D::BeginNewQuadBatch() {
		PX_PROFILE_FUNCTION();

// 		unsigned int size = (unsigned char*)SceneData->SpriteVertexDataPtr - (unsigned char*)SceneData->SpriteVertexDataBase;
// 		SceneData->SpriteVertexBuffer->SetData(SceneData->SpriteVertexDataBase, size);
// 
// 		TextureManager::BindAllTextures();
// 
// 		SceneData->SpriteVertexArray->Bind();
// 		RenderCommand::DrawElements(SceneData->SpriteVertexArray, PimitiveRenderType::Triangles, SceneData->SpriteIndexCount);
// 
// 
// 		SceneData->SpriteIndexCount = 0;
// 		SceneData->SpriteVertexDataPtr = SceneData->SpriteVertexDataBase;
// 		TextureManager::GetManagerData().TextureSlotIndex = 1;

	}

	void Renderer2D::BeginNewLineBatch() {
		PX_PROFILE_FUNCTION();

// 		unsigned int size = (unsigned char*)SceneData->LineQuadVertexData - (unsigned char*)SceneData->LineVertexDataBase;
// 		SceneData->SpriteVertexBuffer->SetData(SceneData->LineVertexDataBase, size);
// 
// 		TextureManager::GetDefaultTexture()->Bind();
// 
// 		SceneData->LineVertexArray->Bind();
// 		RenderCommand::DrawElements(SceneData->LineVertexArray, PimitiveRenderType::Lines, SceneData->LineIndexCount);
// 
// 
// 		SceneData->LineIndexCount = 0;
// 		SceneData->LineQuadVertexData = SceneData->LineVertexDataBase;
// 
// 		TextureManager::GetManagerData().TextureSlotIndex = 1;

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
	}

	
}