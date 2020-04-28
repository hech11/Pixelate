#pragma once

#include "RetroGF/Core/Core.h"

// VertexBuffer and IndexBuffer interface.
// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.


namespace RGF {



	enum class BufferUsage {
		None = -1,
		Static,
		Dynamic
	};

	extern unsigned int GetBufferPlatformUsage(BufferUsage usage);

	enum class BufferLayoutTypes
	{
		None = -1,
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2,
		Int3,
		Int4,
		Short2,
		Short3,
		Short4,
		UChar,
		UChar2,
		UChar3,
		UChar4,
		Char,
		Char2,
		Char3,
		Char4,
		Mat4
	};


	static unsigned int BufferLayoutDataTypeSize(BufferLayoutTypes type)
	{
		switch (type)
		{

			case BufferLayoutTypes::Char: return 1;
			case BufferLayoutTypes::Char2: return 2;
			case BufferLayoutTypes::Char3: return 3;
			case BufferLayoutTypes::Char4: return 4;
			case BufferLayoutTypes::UChar: return 1;
			case BufferLayoutTypes::UChar2: return 2;
			case BufferLayoutTypes::UChar3: return 3;
			case BufferLayoutTypes::UChar4: return 4;
			case BufferLayoutTypes::Float:   return  4;
			case BufferLayoutTypes::Float2:   return 4 * 2;
			case BufferLayoutTypes::Float3:   return 4 * 3;
			case BufferLayoutTypes::Float4:   return 4 * 4;
			case BufferLayoutTypes::Mat4:     return 4 * 4 * 4;
			case BufferLayoutTypes::Int:     return  4;
			case BufferLayoutTypes::Int2:     return 4 * 2;
			case BufferLayoutTypes::Int3:     return 4 * 3;
			case BufferLayoutTypes::Int4:     return 4 * 4;
		}

		RGF_ASSERT(false, "Unknown Buffer layout data type!");
		return 0;
	}



	struct RGF_API BufferElement {
		std::string name; // TODO: Remove this in final builds.
		BufferLayoutTypes type;
		unsigned int count, offset, size;
		bool normilized;

		BufferElement(BufferLayoutTypes dataType, const std::string& dName, bool dnormilized = false)
			: name(dName), type(dataType), size(BufferLayoutDataTypeSize(dataType)), offset(0), normilized(dnormilized)
		{
			count = GetComponentCount();
		}



		unsigned int GetComponentCount() const
		{
			switch (type)
			{
				case BufferLayoutTypes::Char:	  return 1;
				case BufferLayoutTypes::Char2:	  return 2;
				case BufferLayoutTypes::Char3:    return 3;
				case BufferLayoutTypes::Char4:	  return 4;
				case BufferLayoutTypes::UChar:	  return 1;
				case BufferLayoutTypes::UChar2:	  return 2;
				case BufferLayoutTypes::UChar3:    return 3;
				case BufferLayoutTypes::UChar4:	  return 4;
				case BufferLayoutTypes::Float:    return 1;
				case BufferLayoutTypes::Float2:   return 2;
				case BufferLayoutTypes::Float3:   return 3;
				case BufferLayoutTypes::Float4:   return 4;
				case BufferLayoutTypes::Mat4:     return 4 * 4;
				case BufferLayoutTypes::Int:      return 1;
				case BufferLayoutTypes::Int2:     return 2;
				case BufferLayoutTypes::Int3:     return 3;
				case BufferLayoutTypes::Int4:     return 4;
			}
			RGF_ASSERT(false, "Unknown component count!");
			return 0;
		}


	};



	
	class RGF_API BufferLayout {

		public :
			BufferLayout(const std::initializer_list<BufferElement>& elements) 
			 : m_Elements(elements)
			{
				CalculateStrideAndOffset();
			}

			BufferLayout() {}

			void CalculateStrideAndOffset() {
				unsigned int offset = 0;
				m_Stride = 0;

				for (auto& elements : m_Elements) {

					elements.offset = offset;
					offset += elements.size;
					m_Stride += elements.size;
				}
			}


			inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
			inline unsigned int GetStride() const { return m_Stride; }


			std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
			std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
			std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
			std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		private :
			std::vector<BufferElement> m_Elements;
			unsigned int m_Stride = 0;
			
	};





	class RGF_API VertexBuffer {
		public :

			virtual ~VertexBuffer() = default;

			virtual void SetData(const void* data, unsigned int size) = 0;
			
			virtual void SetLayout(const RGF::BufferLayout& layout) = 0;

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;

			virtual const BufferLayout& GetLayout() const = 0;
			virtual BufferLayout& GetLayout() = 0;
			
		public :
			static Ref<VertexBuffer> Create(const void* data, unsigned int size);
			static Ref<VertexBuffer> Create(unsigned int size);

;

	};




	class RGF_API IndexBuffer {

		public :

			virtual ~IndexBuffer() = default;

			virtual void Bind() const = 0;
			virtual void Unbind() const = 0;


			virtual unsigned int GetCount() const = 0;
			virtual unsigned int GetType() const = 0;

		public :
			static Ref<IndexBuffer> Create(unsigned int* data, unsigned int count);
			static Ref<IndexBuffer> Create(unsigned short* data, unsigned int count);
			static Ref<IndexBuffer> Create(unsigned char* data, unsigned int count);
			
	};


}