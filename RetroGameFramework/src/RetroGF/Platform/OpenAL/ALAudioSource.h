#pragma once

#include "RetroGF/Audio/AudioSource.h"


namespace RGF {

	class ALAudioSource : public AudioSource {
		public :
			ALAudioSource(const AudioSourceSpecification& specs);
			virtual ~ALAudioSource();

			virtual bool IsPlaying() override { return m_IsPlaying;  }
			virtual void Play() override;
			virtual void Pause() override;
			virtual void Stop() override;
			virtual void SetLooping(bool loop) override;

			virtual void SetPosition(const glm::vec3& position) override;


			virtual void SetGain(float gain) override;
			virtual void SetSpatial(bool spatial) override;

			virtual unsigned int GetHandleID() override { return m_AudioBufferID; }
			virtual float GetPitch() override { return m_Pitch; }
			virtual float GetGain() override { return m_Gain; };
			virtual bool GetSpatial() override { return m_IsSpatial; }
			virtual bool IsLooping() override { return m_IsLooping; }

		private :
			int DeduceALFormat(int channels, int samples);
		private :
			bool m_IsPlaying = false, m_IsSpatial = false, m_IsLooping = false;
			float m_Gain = 1.0f, m_Pitch = 1.0f;

			// TODO: I should really abstract the audio buffer into its own class. This is for the purpose 
			// to get this to work.
			unsigned int m_AudioBufferID, m_AudioSourceID;

			glm::vec3 m_Position;
			AudioSourceSpecification m_Specification;

			

	};

}

