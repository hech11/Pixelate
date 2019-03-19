#pragma once


#include "RetroGF/Core.h"

#include "RGFpch.h"

namespace RGF {

	struct RGF_API BufferElement {
		unsigned int type, count;
		bool normilized;

		enum GLTYPES : unsigned char{
			Float, 
			Unsigned_Int,
			Unsigned_Short,
			Unsigned_Byte
		};


		static unsigned char ConvertGLTypeToBytes(unsigned int type);
		static unsigned int ConvertBytesToGLType(const GLTYPES& type);
	};

	class RGF_API VertexBufferLayout{
		public :
			VertexBufferLayout(): m_Stride(0){}

			template<typename T>
			void Push(unsigned int count = 1, bool normilized = false) {
				RGF_CORE_CRIT("Unknown Type!!");
			}

			template<>
			void Push<float>(unsigned int count, bool normilized) {
				// If the context is OpenGL.
				m_BufferLayouts.push_back({ BufferElement::ConvertBytesToGLType(BufferElement::GLTYPES::Float), 
					count, false }); // this is only temp...use GL_FLOAT.
				m_Stride += count * sizeof(float);


			}


			template<>
			void Push<unsigned int>(unsigned int count, bool normilized) {
				// If the context is OpenGL.
				m_BufferLayouts.push_back({ BufferElement::ConvertBytesToGLType(BufferElement::GLTYPES::Unsigned_Int),
					count, false });
				m_Stride += count * sizeof(unsigned int);
			}

			template<>
			void Push<unsigned short>(unsigned int count, bool normilized) {
				// If the context is OpenGL.
				m_BufferLayouts.push_back({ BufferElement::ConvertBytesToGLType(BufferElement::GLTYPES::Unsigned_Short),
				count, false });
				m_Stride += count * sizeof(unsigned short);
			}

			template<>
			void Push<unsigned char>(unsigned int count, bool normilized) {
				// If the context is OpenGL.
				m_BufferLayouts.push_back({ BufferElement::ConvertBytesToGLType(BufferElement::GLTYPES::Unsigned_Byte),
				count, false });
				m_Stride += count * sizeof(unsigned char);
			}


			inline const std::vector<BufferElement>& GetElements() const { return m_BufferLayouts; }
			inline unsigned int GetStride() const { return m_Stride; }
		private :
			std::vector<BufferElement> m_BufferLayouts;
			unsigned int m_Stride;
	};

}