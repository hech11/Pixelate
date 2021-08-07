#pragma once

#include <string>
#include "Pixelate/Core/Core.h"
#include "Pixelate/Audio/AudioMixer.h"
#include "GLM/glm/glm.hpp"


#include "AudioBuffer.h"
#include "AudioFilter.h"


namespace Pixelate {
	

	class AudioSource {
		public:
			AudioSource();
			~AudioSource();

			bool IsPlaying() const { return m_IsPlaying; }
			void Play();
			void Pause();
			void Stop();

			void SetLooping(bool loop);

			void SetGain(float gain);
			void SetPitch(float pitch);

			void SetMaxGain(float maxGain);
			void SetMinGain(float minGain);

			void ShouldMute(bool mute);
			void PlayOnAwake(bool awake);

			void ShouldBypassEffects(bool bypass);

			void SetPosition(const glm::vec3& position);
			void SetBufferData(const Ref<AudioBuffer>& buffer);

			void SetLowPassGain(float value);
			void SetHighPassGain(float value);

			void SetMixerGroup(const Ref<AudioMixerGroup>& group) { m_MixerGroup = group; }
			const Ref<AudioMixerGroup>& GetMixerGroup() const { return m_MixerGroup; }

			const Ref<AudioBuffer>& GetBufferData() const;

			Ref<AudioFilter> GetLowPassFilter() const { return m_LowPassFilter; }
			Ref<AudioFilter> GetHighPassFilter() const { return m_HighPassFilter; }

			unsigned int GetAudioSourceHandleID() { return m_AudioSourceID; }

			float GetPitch() const { return m_Pitch; }
			float GetGain() const { return m_Gain;}

			bool IsLooping() const { return m_IsLooping; }
			bool ShouldPlayOnAwake() const { return m_PlayOnAwake; }

			AudioMixerStates GetCurrentState() const { return m_State; }

			


		private:
			bool m_IsPlaying = false, m_PlayOnAwake = false, m_IsLooping = false;
			float m_Gain = 1.0f, m_Pitch = 1.0f;
			float m_MaxGain = 1.0f, m_MinGain = 0.0f;

			AudioMixerStates m_State;

			// TODO: Make them optional instead of forcing all sources to have filters?
			Ref<AudioFilter> m_LowPassFilter;
			Ref<AudioFilter> m_HighPassFilter;
	
			unsigned int m_AudioSourceID;
			glm::vec3 m_Position;
			Ref<AudioBuffer> m_AudioBuffer;

			Ref<AudioMixerGroup> m_MixerGroup;
	
	};

	
}