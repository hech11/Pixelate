#pragma once


#include "Pixelate/Rendering/API/Buffer.h"

// OpenGL's implementation of Vertex and Index buffer


namespace Pixelate {


	class PX_API GLVertexBuffer : public VertexBuffer {
		public :
			GLVertexBuffer(const void* data, unsigned int size);
			GLVertexBuffer(unsigned int size);

			unsigned int GetRendererID() const override { return m_RendererID; }

			virtual ~GLVertexBuffer();


			void SetData(void* data, unsigned int size) override;
			void SetLayout(const BufferLayout& layout) override;


			const BufferLayout& GetLayout() const override { return m_Layout; }
			BufferLayout& GetLayout() override { return m_Layout; }

			void Bind() const override;
			void Unbind() const override;

			
		private :

			struct VertexBufferStatistics {
				unsigned int Size;
				BufferUsage Usage;
			};
			VertexBufferStatistics m_Stats;

			BufferLayout m_Layout;
			unsigned int m_RendererID;

	};




	class PX_API GLIndexBuffer : public IndexBuffer{
		public :

			GLIndexBuffer(unsigned int* data, unsigned int count);
			GLIndexBuffer(unsigned short* data, unsigned int count);
			GLIndexBuffer(unsigned char* data, unsigned int count);


			virtual ~GLIndexBuffer();

			unsigned int GetRendererID() const override { return m_RendererID; }


			unsigned int GetCount() const override{ return m_Count; };
			unsigned int GetType() const override { return m_Type; };


			virtual void Bind() const override;
			virtual void Unbind() const override;


		private :
			unsigned int m_RendererID;

			unsigned int m_Count;
			unsigned int m_Type;

	};


	class GLUniformBuffer : public UniformBuffer
	{

		public:
			GLUniformBuffer(uint32_t size, uint32_t binding);
			~GLUniformBuffer();

			void SetData(const void* data, uint32_t size, uint32_t offset) override;
			void* GetData(uint32_t offset) override;

			uint32_t GetRendererID() const override { return m_RendererID; }
			uint32_t GetBinding() const override { return m_Binding; }

			void Bind() const override;
			void UnBind() const override;

		private:
			uint32_t m_RendererID;
			uint32_t m_Binding;


			char* m_Data;
			uint32_t m_Size;
	};

}