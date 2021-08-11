#pragma once

#include <string>
#include "Pixelate/Core/Core.h"
#include "GLM/glm/glm.hpp"


#include "AudioBuffer.h"
#include "AudioFilter.h"

#include "AudioMixer.h"

#include "Pixelate/Audio/DSP/Filters/BandPass.h"
#include "Pixelate/Audio/DSP/Filters/HighPass.h"
#include "Pixelate/Audio/DSP/Filters/LowPass.h"

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


			void ShouldMute(bool mute);
			void PlayOnAwake(bool awake);

			void ShouldBypassEffects(bool bypass);

			void SetPosition(const glm::vec3& position);
			void SetBufferData(const Ref<AudioBuffer>& buffer);

			void AddFilter(const AudioFilterType& Filter) {
				switch (Filter)
				{
					case AudioFilterType::LowPass:
						if (!m_LowpassFilter) {
							m_LowpassFilter = CreateRef<LowPass>();
						}
						else {
							PX_CORE_WARN("There is already a low-pass filter added to this audio source!\n");
						}
						break;
					case AudioFilterType::HighPass:
						if (!m_HighpassFilter) {
							m_HighpassFilter = CreateRef<HighPass>();
						}
						else {
							PX_CORE_WARN("There is already a high-pass filter added to this audio source!\n");
						}
						break;
					case AudioFilterType::BandPass:
						if (!m_BandpassFilter) {
							m_BandpassFilter = CreateRef<BandPass>();
						}
						else {
							PX_CORE_WARN("There is already a bandpass filter added to this audio source!\n");
						}
						break;
					default:
						break;

				}

			}

			const Ref<LowPass>& GetLowPassFilter() const { return m_LowpassFilter; }
			const Ref<HighPass>& GetHighPassFilter() const { return m_HighpassFilter; }
			const Ref<BandPass>& GetBandPassFilter() const { return m_BandpassFilter; }

			void ApplyFilterChanges();

			void SetMixerGroup(const Ref<AudioMixerGroup>& group) { m_MixerGroup = group; }
			const Ref<AudioMixerGroup>& GetMixerGroup() const { return m_MixerGroup; }

			const Ref<AudioBuffer>& GetBufferData() const;


			unsigned int GetAudioSourceHandleID() { return m_AudioSourceID; }

			float GetPitch() const { return m_Pitch; }
			float GetGain() const { return m_Gain;}

			bool IsLooping() const { return m_IsLooping; }
			bool ShouldPlayOnAwake() const { return m_PlayOnAwake; }

			AudioMixerStates GetCurrentState() const { return m_State; }

			


		private:
			bool m_IsPlaying = false, m_PlayOnAwake = false, m_IsLooping = false;
			float m_Gain = 0.1f, m_Pitch = 1.0f;

			AudioMixerStates m_State;

			Ref<BandPass> m_BandpassFilter;
			Ref<HighPass> m_HighpassFilter;
			Ref<LowPass> m_LowpassFilter;
	
			unsigned int m_AudioSourceID;
			glm::vec3 m_Position;
			Ref<AudioBuffer> m_AudioBuffer;

			Ref<AudioMixerGroup> m_MixerGroup;
	
	};


	
}