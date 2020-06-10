#include "RGFpch.h"
#include "Listener.h"

#include "RetroGF/Audio/AudioContext.h"
#include "RetroGF/Platform/OpenAL/ALListener.h"

namespace RGF {

	Ref<Listener> Listener::Create() {
		switch (AudioContext::GetContext()) {
			case AudioContext::ContextAPI::OPENAL:
				return CreateRef<ALListener>();

		}
	}

}