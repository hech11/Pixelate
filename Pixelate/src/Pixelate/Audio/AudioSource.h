#pragma once

#include <string>
#include "Pixelate/Core/Core.h"
#include "GLM/glm/glm.hpp"


#include "AudioBuffer.h"


namespace Pixelate {
	

	class AudioSource {
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

			const Ref<AudioBuffer>& GetBufferData() const;

			unsigned int GetAudioSourceHandleID() { return m_AudioSourceID; }
			float GetPitch() { return m_Pitch; }
			float GetGain() { return m_Gain;}
			bool IsLooping() { return m_IsLooping; }



		private:
			bool m_IsPlaying = false, m_IsSpatial = false, m_IsLooping = false;
			float m_Gain = 1.0f, m_Pitch = 1.0f;
	
			unsigned int m_AudioSourceID;
			glm::vec3 m_Position;
			Ref<AudioBuffer> m_AudioBuffer;
	
	};

	

}