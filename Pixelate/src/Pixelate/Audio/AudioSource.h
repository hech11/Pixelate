#pragma once

#include <string>
#include "Pixelate/Core/Core.h"
#include "GLM/glm/glm.hpp"


#include "AudioBuffer.h"

#include "Pixelate/Asset/Asset.h"

// Audio source contains both buffer and the source audio data. In the future I may want to separate the buffer and the audio
// source into different classes

namespace Pixelate {
	

	class AudioSource : public Asset {
		public:
			AudioSource();
			~AudioSource();

			bool IsPlaying() { return m_IsPlaying; }
			void Play();
			void Pause();
			void Stop();
			void SetLooping(bool loop);

			void SetGain(float gain);

			void SetPosition(const glm::vec3& position);
			void SetBufferData(const Ref<AudioBuffer>& buffer);


			unsigned int GetAudioSourceHandleID() { return m_AudioSourceID; }
			float GetPitch() { return m_Pitch; }
			float GetGain() { return m_Gain;}
			bool IsLooping() { return m_IsLooping; }

			SETUP_ASSET_PROPERTIES(AssetType::Audio);


		private:
			bool m_IsPlaying = false, m_IsSpatial = false, m_IsLooping = false;
			float m_Gain = 1.0f, m_Pitch = 1.0f;
	
			unsigned int m_AudioSourceID;
			glm::vec3 m_Position;
	
	};


}