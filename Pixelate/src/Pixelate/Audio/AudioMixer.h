#pragma once

#include "Pixelate/Core/Core.h"
#include "AudioMixerStates.h"

#include <vector>
#include <string>

#include "Pixelate/Asset/Asset.h"


namespace Pixelate {


	struct AudioMixerGroup {

		float Gain = 0.1f;
		AudioMixerStates State = AudioMixerStates::Default;
		std::string DebugName;

	};

	class AudioMixer : public Asset {
		public :
			AudioMixer();

			const Ref<AudioMixerGroup>& const GetMasterGroup() { return m_MasterGroup; }
			std::vector<Ref<AudioMixerGroup>>& const GetGroups() { return m_Groups; }

			SETUP_ASSET_PROPERTIES(AssetType::AudioMixer);
		private :

			Ref<AudioMixerGroup> m_MasterGroup;
			std::vector<Ref<AudioMixerGroup>> m_Groups;
	};

}