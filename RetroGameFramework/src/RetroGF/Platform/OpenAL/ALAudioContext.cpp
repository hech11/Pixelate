#include "RGFpch.h"
#include "RetroGF/Audio/AudioContext.h"

#include "OPENAL_SOFT/include/AL/al.h"
#include "OPENAL_SOFT/include/AL/alc.h"

#include "OPENAL_SOFT/include/AL/alext.h"
#include "OPENAL_SOFT/src/alc/alcmain.h"

#include "ALCommon.h"

namespace RGF {

	// Do more research into this...
	static ALCcontext* s_Context = nullptr; // Not sure if this should be defined here.. but this works for now
	static ALCdevice* s_AudioDevice = nullptr; // Not sure if this should be defined here.. but this works for now



	void AudioContext::Init() {

		m_Context = ContextAPI::OpenAL;

		s_AudioDevice = alcOpenDevice(NULL);

		if (!s_AudioDevice)
			RGF_ASSERT(false, "Could not open an audio device!");

		s_Context = alcCreateContext(s_AudioDevice, NULL);
		if (s_Context == nullptr || alcMakeContextCurrent(s_Context) == ALC_FALSE)
		{
			if (s_Context != nullptr)
				alcDestroyContext(s_Context);
			alcCloseDevice(s_AudioDevice);
			RGF_ASSERT(false, "Could not set a context!");
		}

		// print device information..
		
		RGF_CORE_TRACE("----- Audio device information -----\n");
		RGF_CORE_TRACE("Device name: %s\n", s_AudioDevice->DeviceName.c_str());
		RGF_CORE_TRACE("Sample Rate: %d\n", s_AudioDevice->Frequency);
		RGF_CORE_TRACE("Available Mono: %d\n", s_AudioDevice->NumMonoSources);
		RGF_CORE_TRACE("Available Stereo: %d\n", s_AudioDevice->NumStereoSources);
		RGF_CORE_TRACE("--------------------------\n\n\n\n\n");


	}

	void AudioContext::Close() {
		alcDestroyContext(s_Context);
		alcCloseDevice(s_AudioDevice);
	}


}
