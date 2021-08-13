#pragma once

#include "Pixelate/Core/Core.h"


namespace Pixelate {

	enum AudioMixerStates {
		Default = 0, Solo = BIT(1), Mute = BIT(2), Bypass = BIT(3)
	};

	inline AudioMixerStates operator|(AudioMixerStates l, AudioMixerStates r) {
		return static_cast<AudioMixerStates>((int)l | (int)r);
	}

	inline AudioMixerStates operator&(AudioMixerStates l, AudioMixerStates r) {
		return static_cast<AudioMixerStates>((int)l & (int)r);
	}


}