#pragma once


#include "Pixelate/Audio/AudioLoader.h"
#include "Pixelate/Asset/Asset.h"


namespace Pixelate {

	class AudioBuffer : public Asset {

		public:

			AudioBuffer();
			AudioBuffer(const AudioFileSpecification& spec);

			~AudioBuffer();

			void SetData(const AudioFileSpecification& spec);

			unsigned int GetHandleID() const { return m_AudioID; }

			SETUP_ASSET_PROPERTIES(AssetType::Audio);

		private:
			unsigned int m_AudioID;
	};
}