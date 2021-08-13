#pragma once

#include "AudioMixer.h"
#include "Pixelate/Utility/FileSystem.h"


#include <yaml-cpp/yaml.h>

namespace Pixelate {


	class AudioMixerImporter {

		public :
			static Ref<AudioMixer> Import(const std::filesystem::path& path);
			static void Export(const std::filesystem::path& path, const Ref<AudioMixer>& mixer);
		private :
			static void ExportGroup(const Ref<AudioMixerGroup>& group, YAML::Emitter& data);
	};


}