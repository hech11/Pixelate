#pragma once



#include "Pixelate/Core/Log.h"
#include "Pixelate/Core/Core.h"
#include <OPENAL_SOFT/include/AL/al.h>

// This is used to debug OpenAL code.

#ifdef PX_DISTRIBUTE
#define ALCall(x) x
#else 
 // This is to be wrapped around every openal code. Example : AlCall(....);

#define ALCall(x) x;\
PX_ASSERT(Pixelate::AL::ALLogCall(#x, __FILE__, __LINE__), "");

#endif

namespace Pixelate { namespace AL {

	static void ALClearError() {
		while (alGetError() != AL_NO_ERROR);
	}
	static const char* GetALErrorName(const ALenum& error) {
		switch (error) {
		case AL_INVALID_ENUM:
			return "AL_INVALID_ENUM";
		case AL_INVALID_OPERATION:
			return "AL_INVALID_OPERATION";
		case AL_OUT_OF_MEMORY:
			return "AL_OUT_OF_MEMORY";
		default:
			return "no error listed!";
		}

		return "no error listed!";
	}

	static bool ALLogCall(const char* function, const char* file, int line) {
		ALenum error;
		if ((error = alGetError()) != AL_NO_ERROR) {
			PX_CORE_ERROR("OpenAL Error! : %s", GetALErrorName(error));
			return false;
		}

		return true;
	}
	

} }