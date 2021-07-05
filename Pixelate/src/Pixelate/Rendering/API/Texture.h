#pragma once

#include "Pixelate/Core/Core.h"

#include "PXpch.h"
#include <GLM/glm/glm.hpp>


// Texture interface.
// The 'Create' method will decide depending on the API choice. OpenGL, Directx 11 or 12, vulkan etc.

namespace Pixelate {
	// A abstracted way to format the textures.
	// The abstraction code should be implemented in the API texture's ".cpp" file.


	struct TextureProperties {
		enum class Format {
			None = -1,
			RGBA,
			RGB
		};
		enum class Filter {
			None = -1,
			Nearest,
			Linear,
		};
		enum class Wrap {
			None = -1,
			Clamp_To_Edge,
			Repeat
		};

		Format TexFormat = Format::RGBA;
		Filter TexFilter = Filter::Nearest;
		Wrap TexWrap = Wrap::Clamp_To_Edge;
		bool GenerateMipMaps = false;

		int Width = 0, Height = 0, BPP = 0;
		mutable unsigned char Slot = 0;

		mutable bool IsBound = false;
		std::string FilePath;
	};




	class PX_API Texture {

		public :
					
			Texture() {}
			virtual ~Texture(){}

			virtual void Bind(unsigned char slot = 0) const = 0;
			virtual void Unbind() const = 0;


			virtual void SetProperties(TextureProperties& props) = 0;

			//TODO: Set data only sets the whole texture.
			virtual void SetData(void* data, unsigned int size) = 0;
			virtual void SetData(const std::string& filepath) = 0;

			virtual unsigned int GetHandleID() const = 0;


			//TODO: Should this be stored in the platform specific classes or in this class?
			inline int GetWidth() const { return m_Props.Width; }
			inline int GetHeight() const { return m_Props.Height; }
			inline int GetBPP() const { return m_Props.BPP; }
			inline unsigned char GetCurrentSlot() const { return m_Props.Slot; }
			inline bool IsBound() const { return m_Props.IsBound; }
			inline std::string& GetFilepath() { return m_Props.FilePath;  }

			virtual const TextureProperties& GetTextureProps() const = 0;

			virtual bool operator==(const Texture& other) const = 0;

		public :
			static Ref<Texture> Create(const std::string& filepath, TextureProperties props = TextureProperties());
			static Ref<Texture> Create(unsigned int width, unsigned int height, TextureProperties::Format format = TextureProperties::Format::RGBA);

		protected :
			TextureProperties m_Props;
			
	};

}