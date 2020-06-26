#include "RGFpch.h"

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"

#include "ReadAudioFiles.h"

#include <filesystem>

namespace RGF {

	AudioFileSpecs::FileFormat AudioFileSpecs::DeduceFileFormat(const std::string& filepath) {
		std::filesystem::path path = filepath;
		std::string extension = path.extension().string();

		if (extension == ".wav")  return FileFormat::Wav;
		if (extension == ".mp3")  return FileFormat::Mp3;


		RGF_WARN("Could not deduce audio format!");
		return FileFormat::None;
	}

	AudioFileSpecs AudioFileSpecs::LoadAudioData(const std::string& filepath) {
		AudioFileSpecs specs;
		specs.Extention = DeduceFileFormat(filepath);

		if (specs.Extention == FileFormat::Mp3) {
			MP3Header header;
			header.LoadData(filepath);
			return header;
		} else if (specs.Extention == FileFormat::Wav) {
			WavFileHeader header;
			header.LoadData(filepath);
			return header;
		}



	}



	//////////////////////////////// -- WavFileHeader -- ////////////////////////////////
	// This is unsafe code.
	void WavFileHeader::LoadData(const std::string& filepath)
	{
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
	///////////////////////////////////////////////////////////////////////////


	//////////////////////////////// -- MP3Header -- ////////////////////////////////

	mp3dec_t MP3Header::mp3d;

	MP3Header::MP3Header()
	{
		static bool init = false;
		if (!init) {
			mp3dec_init(&mp3d);
			init = true;
		}
	}


	void MP3Header::LoadData(const std::string& filepath) {
		mp3dec_file_info_t info;
		int result = mp3dec_load(&mp3d, filepath.c_str(), &info, 0, 0);


		Size = info.samples * sizeof(mp3d_sample_t);
		Bps = Size / (info.avg_bitrate_kbps * 1024.0f);
		SampleRate = info.hz;
		Channels = info.channels;



		Data = (void*)info.buffer;
	}
	///////////////////////////////////////////////////////////////////////////


}