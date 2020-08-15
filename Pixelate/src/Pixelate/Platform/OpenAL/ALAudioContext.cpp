#include "PXpch.h"
#include "Pixelate/Audio/AudioContext.h"

#include "OPENAL_SOFT/include/AL/al.h"
#include "OPENAL_SOFT/include/AL/alc.h"

#include "OPENAL_SOFT/include/AL/alext.h"
#include "OPENAL_SOFT/src/alc/alcmain.h"

#include "ALCommon.h"

#include "Pixelate/Debug/Instrumentor.h"

namespace Pixelate {

	// Do more research into this...
	static ALCcontext* s_Context = nullptr; // Not sure if this should be defined here.. but this works for now
	static ALCdevice* s_AudioDevice = nullptr; // Not sure if this should be defined here.. but this works for now



	void AudioContext::Init() {
		PX_PROFILE_FUNCTION();

		m_Context = ContextAPI::OpenAL;

		s_AudioDevice = alcOpenDevice(NULL);

		if (!s_AudioDevice)
			PX_ASSERT(false, "Could not open an audio device!");

		s_Context = alcCreateContext(s_AudioDevice, NULL);
		if (s_Context == nullptr || alcMakeContextCurrent(s_Context) == ALC_FALSE)
		{
			if (s_Context != nullptr)
				alcDestroyContext(s_Context);
			alcCloseDevice(s_AudioDevice);
			PX_ASSERT(false, "Could not set a context!");
		}

		// print device information..
		PX_CORE_MSG("Audio context: OpenAL\n");
		PX_CORE_TRACE("----- Information -----\n");
		PX_CORE_TRACE("Device name: %s\n", s_AudioDevice->DeviceName.c_str());
		PX_CORE_TRACE("Sample Rate: %d\n", s_AudioDevice->Frequency);
		PX_CORE_TRACE("Available Mono: %d\n", s_AudioDevice->NumMonoSources);
		PX_CORE_TRACE("Available Stereo: %d\n", s_AudioDevice->NumStereoSources);
		PX_CORE_TRACE("--------------------------\n\n");


	}

	void AudioContext::Close() {
		PX_PROFILE_FUNCTION();

		alcDestroyContext(s_Context);
		alcCloseDevice(s_AudioDevice);
	}


}
