#pragma once

#include "RetroGF/Core/Core.h"

#include "RGFpch.h"


// Texture interface.

// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.

namespace RGF {
	// A abstracted way to format the textures.
	// The abstraction code should be implemented in the API texture's ".cpp" file.

	enum class TextureFormat {
		None = -1,
		RGBA,
		RGB
	};

	enum class TextureFilter {
		None = -1,
		Nearest,
		Linear,
	};

	enum class TextureWrap {
		None = -1,
		Clamp_To_Edge,
		Repeat
	};

	struct TextureParameters {

		TextureParameters(TextureFormat format, TextureFilter filter, TextureWrap wrap) 
			: Format(format), Filter(filter), Wrap(wrap) 
		{}

		TextureParameters(){}


		TextureFormat Format = TextureFormat::RGBA;
		TextureFilter Filter = TextureFilter::Nearest;
		TextureWrap Wrap = TextureWrap::Clamp_To_Edge;

	};




	class RGF_API Texture {

		public :

			Texture() {}
			virtual ~Texture(){}

			virtual void Bind(unsigned char slot = 0) const = 0;
			virtual void Unbind() const = 0;

			virtual void LoadTexture(const std::string& filepath) = 0;

			virtual unsigned int GetHandleID() const = 0;

			inline int GetWidth() const { return m_Width; }
			inline int GetHeight() const { return m_Height; }
			inline int GetBPP() const { return m_BPP; }

		public :
			static Ref<Texture> Create(unsigned int width, unsigned int height, TextureParameters params);

		protected :
			TextureParameters m_Params;
			int m_Width, m_Height, m_BPP;
			mutable unsigned char m_Slot;
	};

}