#include "RGFpch.h"

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"

#include "AudioFormat.h"

#include <filesystem>


#include "VORBIS/include/vorbis/codec.h"
#include "VORBIS/include/vorbis/vorbisfile.h"

#include "STB/VORBIS/stb_vorbis.h"

namespace RGF {

	AudioFormatSpec::FileFormat AudioFormatSpec::DeduceFileFormat(const std::string& filepath) {
		std::filesystem::path path = filepath;
		std::string extension = path.extension().string();

		if (extension == ".wav")  return FileFormat::Wav;
		if (extension == ".mp3")  return FileFormat::Mp3;
		if (extension == ".ogg")  return FileFormat::Ogg;


		RGF_WARN("Could not deduce audio format!");
		return FileFormat::None;
	}

	AudioFormatSpec AudioFormatSpec::LoadAudioData(const std::string& filepath) {
		AudioFormatSpec specs;
		specs.Extention = DeduceFileFormat(filepath);

		if (specs.Extention == FileFormat::Mp3) {
			MP3Format format;
			format.LoadData(filepath);
			return format;
		} else if (specs.Extention == FileFormat::Wav) {
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



	//////////////////////////////// -- WavFormat -- ////////////////////////////////
	// This is unsafe code.
	void WavFormat::LoadData(const std::string& filepath)
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


	//////////////////////////////// -- MP3Format -- ////////////////////////////////

	mp3dec_t MP3Format::mp3d;

	MP3Format::MP3Format()
	{
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
	///////////////////////////////////////////////////////////////////////////


	//////////////////////////////// -- OggFormat -- ////////////////////////////////


	void OggFormat::LoadData(const std::string& filepath) {


		short *dat;
		
		int samplesDecoded = stb_vorbis_decode_filename(filepath.c_str(), &Channels, &SampleRate, &dat);
		if (samplesDecoded == -1) {
			RGF_CORE_ERROR("There was an error decoding samples at: %s", filepath.c_str());
		}

		Data = dat;
		// I am guessing how you retrieve the size. 2 bytes per sample as dat
		// is a short and a short is 2 bytes typically...
		Size = 2 * Channels * samplesDecoded;
		


		// This code used vorbis and lib ogg to decode.
		// TODO: Remove lib ogg and vorbis libraries
		/*

		FILE* f = fopen(filepath.c_str(), "rb");
		OggVorbis_File ovFile;

		if (ov_open_callbacks(f, &ovFile, 0, 0, OV_CALLBACKS_NOCLOSE) < 0) {
			RGF_ERROR("Could not open ogg stream!\n");
		}

		vorbis_info* vi = ov_info(&ovFile, -1);
		SampleRate = vi->rate;
		Channels = vi->channels;
		
		unsigned long long samples = ov_pcm_total(&ovFile, -1);
		unsigned int bufferSize = 2 * Channels * samples;


		if (ScratchBufferSize < bufferSize) {
			ScratchBufferSize = bufferSize;
		}

		ScratchBufferTemp = new unsigned char[ScratchBufferSize];

		unsigned char* oggBuffer = ScratchBufferTemp;
		unsigned char* bufferPtr = oggBuffer;
		int eof = 0;
		while (!eof) {
			int current_section;
			long length = ov_read(&ovFile, (char*)bufferPtr, bufferSize, 0, 2, 1, &current_section);
			bufferPtr += length;
			if (length == 0) {
				eof = 1;
			}
			else if (length < 0) {
				if (length == OV_EBADLINK) {
					RGF_ASSERT(false, "Corrupt bitstream!");
				}
			}
		}

		Size = bufferPtr - oggBuffer;
		RGF_ASSERT(bufferSize == Size, "Buffer size is not correct!");
		Data = oggBuffer;


		ov_clear(&ovFile);
		fclose(f);
		delete[] ScratchBufferTemp;

		*/

	}
	///////////////////////////////////////////////////////////////////////////

}