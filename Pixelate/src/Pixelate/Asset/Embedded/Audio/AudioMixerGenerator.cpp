#include "PXpch.h"
#include "AudioMixerGenerator.h"
#include "Pixelate/Utility/Filesystem.h"


namespace Pixelate {


	static const char* buffer = R"(Master group:
		Gain: 1
		State : 0
		Mixer groups :
	[] )";

	static const char* DefaultFilename = "Default Mixer.pxam";



	void AudioMixerGenerator::GenerateToDisk(std::filesystem::path path) {

		// TODO: If file already exists

		path /= DefaultFilename;
		std::ofstream file(path);
		file << buffer;
		file.close();
	}

}