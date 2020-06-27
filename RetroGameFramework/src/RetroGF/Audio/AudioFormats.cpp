#include "RGFpch.h"

#include "AudioBuffer.h"

#include <string>
#include <fstream>

#include "RetroGF/Utility/Log.h"

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include "minimp3_ex.h"

#include <filesystem>
#include "STB/VORBIS/stb_vorbis.h"


namespace RGF {


	////////////////////////--AudioFormatSpec--///////////////////////////////
	
		
	AudioFormatSpec AudioFormatSpec::LoadAudioData(const std::string& filepath) {
		AudioFormatSpec specs;
		specs.Extention = DeduceFileFormat(filepath);

		switch (specs.Extention) {
			case FileFormat::Mp3:
				MP3Format::LoadData(filepath, &specs);
				break;
			case FileFormat::Wav:
				WavFormat::LoadData(filepath, &specs);
				break;
			case FileFormat::Ogg:
				OggFormat::LoadData(filepath, &specs);
				break;

			default:
				RGF_ASSERT(false, "audio file type not supported!");
		}

		return specs;



	}


	AudioFormatSpec::FileFormat AudioFormatSpec::DeduceFileFormat(const std::string& filepath) {
		std::filesystem::path path = filepath;
		std::string extension = path.extension().string();

		if (extension == ".wav")  return FileFormat::Wav;
		if (extension == ".mp3")  return FileFormat::Mp3;
		if (extension == ".ogg")  return FileFormat::Ogg;


		RGF_WARN("Could not deduce audio format!");
		return FileFormat::None;

	}
	//////////////////////////////////////////////////////////////////////////

	/////////////////////////-- WavFormat --//////////////////////////////////

	void WavFormat::LoadData(const std::string& filepath, AudioFormatSpec* specs) {
		char buffer[4];
		std::ifstream in(filepath.c_str(), std::ios::binary);
		in.read(buffer, 4);
		if (strncmp(buffer, "RIFF", 4) != 0) {
			RGF_ASSERT(false, "this is not a valid WAVE file");
		}

		in.read(buffer, 4);
		in.read(buffer, 4);      //WAVE
		in.read(buffer, 4);      //fmt
		in.read(buffer, 4);      //16
		in.read(buffer, 2);      //1
		in.read(buffer, 2);
		specs->Channels = ConvertToInt(buffer, 2);
		in.read(buffer, 4);
		specs->SampleRate = ConvertToInt(buffer, 4);
		in.read(buffer, 4);
		in.read(buffer, 2);
		in.read(buffer, 2);
		specs->Bps = ConvertToInt(buffer, 2);
		in.read(buffer, 4);      //data
		in.read(buffer, 4);
		specs->Size = ConvertToInt(buffer, 4);
		specs->Data = (char*)alloca(specs->Size);
		in.read((char*)specs->Data, specs->Size);
	}
	//////////////////////////////////////////////////////////////////////////

	////////////////////////////-- MP3Format --///////////////////////////////


	mp3dec_t MP3Format::mp3d;

	MP3Format::MP3Format() {
		static bool init = false;
		if (!init) {
			mp3dec_init(&mp3d);
			init = true;
		}
	}
	void MP3Format::LoadData(const std::string& filepath, AudioFormatSpec* specs) {
		mp3dec_file_info_t info;
		int result = mp3dec_load(&mp3d, filepath.c_str(), &info, 0, 0);


		specs->Size = info.samples * sizeof(mp3d_sample_t);
		specs->Bps = specs->Size / (info.avg_bitrate_kbps * 1024.0f);
		specs->SampleRate = info.hz;
		specs->Channels = info.channels;



		specs->Data = (void*)info.buffer;
	}
	//////////////////////////////////////////////////////////////////////////


	///////////////////////////-- OggFomrat --///////////////////////////////

	void OggFormat::LoadData(const std::string& filepath, AudioFormatSpec* specs) {
		short* dat;

		int samplesDecoded = stb_vorbis_decode_filename(filepath.c_str(), &specs->Channels, &specs->SampleRate, &dat);
		if (samplesDecoded == -1) {
			RGF_CORE_ERROR("There was an error decoding samples at: %s", filepath.c_str());
		}

		specs->Data = dat;
		// I am guessing how you retrieve the size. 2 bytes per sample as dat
		// is a short and a short is 2 bytes typically...
		specs->Size = 2 * specs->Channels * samplesDecoded;
	}
	//////////////////////////////////////////////////////////////////////////

}