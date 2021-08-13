#include "PXpch.h"
#include "AudioMixerImporter.h"



namespace Pixelate {


	void AudioMixerImporter::ExportGroup(const Ref<AudioMixerGroup>& group, YAML::Emitter& data) {
		data << YAML::BeginMap;
		data << YAML::Key << "Name" << YAML::Value << group->DebugName;
		data << YAML::Key << "Gain" << YAML::Value << group->Gain;
		data << YAML::Key << "State" << YAML::Value << (int)group->State;
		data << YAML::EndMap;


	}



	Ref<AudioMixer> AudioMixerImporter::Import(const std::filesystem::path& path) {

		std::ifstream file(path);
		YAML::Node data = YAML::Load(file);

		Ref<AudioMixer> result = CreateRef<AudioMixer>();

		if (!data["Master group"]) {
			PX_CORE_ERROR("Could not load audio mixer!\n");
			return nullptr;
		}

		YAML::Node masterNode = data["Master group"];
		if (masterNode) {

			float gain = masterNode["Gain"].as<float>();
			int state = masterNode["State"].as<int>();

			result->GetMasterGroup()->DebugName = "Master";
			result->GetMasterGroup()->Gain = gain;
			result->GetMasterGroup()->State = (AudioMixerStates)state;

		}

		YAML::Node mixerGroups = data["Mixer groups"];

		if (mixerGroups) {

			for (auto group : mixerGroups) {
				Ref<AudioMixerGroup> resultGroup = CreateRef<AudioMixerGroup>();
				std::string name = group["Name"].as<std::string>();
				float gain = group["Gain"].as<float>();
				int state = group["State"].as<int>();

				resultGroup->DebugName = name;
				resultGroup->Gain = gain;
				resultGroup->State = (AudioMixerStates)state;

				result->GetGroups().push_back(resultGroup);
			}

		}

		file.close();
		return result;

	}


	void AudioMixerImporter::Export(const std::filesystem::path& path, const Ref<AudioMixer>& mixer) {

		const Ref<AudioMixerGroup>& masterGroup = mixer->GetMasterGroup();
		const auto& groups = mixer->GetGroups();

		YAML::Emitter data;
		data << YAML::BeginMap;


		data << YAML::Key << "Master group";

		data << YAML::BeginMap;
		data << YAML::Key << "Gain" << YAML::Value << masterGroup->Gain;
		data << YAML::Key << "State" << YAML::Value << (int)masterGroup->State;
		data << YAML::EndMap;



		data << YAML::Key << "Mixer groups";
		data << YAML::Value << YAML::BeginSeq;

		for (auto& group : groups) {
			ExportGroup(group, data);
		}

		data << YAML::EndSeq;
		data << YAML::EndMap;

		std::ofstream file(path);
		file << data.c_str();

		file.close();


	}


}