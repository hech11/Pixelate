#pragma once

#include "Pixelate/Core/Core.h"

#include <vector>
#include <string>

namespace Pixelate {

	enum class AudioMixerStates { Default, Solo, Mute, Bypass };

	struct AudioMixerGroup {

		float Gain = 0.1f;
		AudioMixerStates State = AudioMixerStates::Default;
		std::string DebugName;

	};

	class AudioMixer {
		public :
			AudioMixer();

			const Ref<AudioMixerGroup>& const GetMasterGroup() { return m_MasterGroup; }
			std::vector<Ref<AudioMixerGroup>>& const GetGroups() { return m_Groups; }

		private :
			Ref<AudioMixerGroup> m_MasterGroup;
			std::vector<Ref<AudioMixerGroup>> m_Groups;
	};

}