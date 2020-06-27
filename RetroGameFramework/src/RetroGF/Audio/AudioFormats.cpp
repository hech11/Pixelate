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

		if (specs.Extention == FileFormat::Mp3) {
			MP3Format format;
			format.LoadData(filepath);
			return format;
		}
		else if (specs.Extention == FileFormat::Wav) {
			WavFormat format;
			format.LoadData(filepath);
			return format;
		}
		else if (specs.Extention == FileFormat::Ogg) {
			OggFormat format;
			format.LoadData(filepath);
			return format;
		}



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

	void WavFormat::LoadData(const std::string& filepath) {
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
		Channels = ConvertToInt(buffer, 2);
		in.read(buffer, 4);
		SampleRate = ConvertToInt(buffer, 4);
		in.read(buffer, 4);
		in.read(buffer, 2);
		in.read(buffer, 2);
		Bps = ConvertToInt(buffer, 2);
		in.read(buffer, 4);      //data
		in.read(buffer, 4);
		Size = ConvertToInt(buffer, 4);
		Data = (char*)alloca(Size);
		in.read((char*)Data, Size);
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
	void MP3Format::LoadData(const std::string& filepath) {
		mp3dec_file_info_t info;
		int result = mp3dec_load(&mp3d, filepath.c_str(), &info, 0, 0);


		Size = info.samples * sizeof(mp3d_sample_t);
		Bps = Size / (info.avg_bitrate_kbps * 1024.0f);
		SampleRate = info.hz;
		Channels = info.channels;



		Data = (void*)info.buffer;
	}
	//////////////////////////////////////////////////////////////////////////


	///////////////////////////-- OggFomrat --///////////////////////////////

	void OggFormat::LoadData(const std::string& filepath) {
		short* dat;

		int samplesDecoded = stb_vorbis_decode_filename(filepath.c_str(), &Channels, &SampleRate, &dat);
		if (samplesDecoded == -1) {
			RGF_CORE_ERROR("There was an error decoding samples at: %s", filepath.c_str());
		}

		Data = dat;
		// I am guessing how you retrieve the size. 2 bytes per sample as dat
		// is a short and a short is 2 bytes typically...
		Size = 2 * Channels * samplesDecoded;
	}
	//////////////////////////////////////////////////////////////////////////

}