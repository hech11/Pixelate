#include "RGFpch.h"
#include "ALAudioContext.h"

#include "OPENAL_SOFT/include/AL/al.h"
#include "OPENAL_SOFT/include/AL/alc.h"

#include "OPENAL_SOFT/include/AL/alext.h"
#include "OPENAL_SOFT/src/alc/alcmain.h"

#include "ALCommon.h"

namespace RGF {

	// Do more research into this...
	static ALCcontext* s_Context = nullptr; // Not sure if this should be defined here.. but this works for now
	static ALCdevice* s_AudioDevice = nullptr; // Not sure if this should be defined here.. but this works for now

	void ALAudioContext::Init() {


		ALCall(s_AudioDevice = alcOpenDevice(nullptr));

		if (!s_AudioDevice)
			RGF_ASSERT(false, "Could not open an audio device!");

		ALCall(s_Context = alcCreateContext(s_AudioDevice, 0));
		if (s_Context == nullptr || alcMakeContextCurrent(s_Context) == ALC_FALSE)
		{
			if (s_Context != nullptr)
				ALCall(alcDestroyContext(s_Context));
			ALCall(alcCloseDevice(s_AudioDevice));
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

	void ALAudioContext::Close() {
		ALCall(alcDestroyContext(s_Context));
		ALCall(alcCloseDevice(s_AudioDevice));
	}


}
