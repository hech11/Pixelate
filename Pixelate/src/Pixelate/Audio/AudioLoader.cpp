#include "PXpch.h"
#include "AudioLoader.h"

#include "Pixelate/Debug/Instrumentor.h"

#define MINIMP3_IMPLEMENTATION
#include <minimp3_ex.h>
#include <minimp3.h>

#include <STB/VORBIS/stb_vorbis.h>

#include <filesystem>


namespace Pixelate {

	inline static std::unordered_map<std::string, Ref<Utils::AudioBaseLoader>> s_Loaders = {
		{".ogg", CreateRef<Utils::OggLoader>()},
		{".mp3", CreateRef<Utils::Mp3Loader>()},
		{".wav", CreateRef<Utils::WavLoader>()},

	};

	;
	static mp3dec_t s_mp3Loader;


	AudioFileSpecification AudioLoader::Load(const std::filesystem::path& path)
	{
		std::string ext = path.extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) {return std::tolower(c); });

		if (s_Loaders.find(ext) == s_Loaders.end()) {
			PX_CORE_ERROR("Pixelate does not support this type of audio file!: (%s)\n", ext.c_str());
			return {};
		}

		return s_Loaders[ext]->Load(path);

	}




	namespace Utils {
		AudioFileSpecification WavLoader::Load(const std::filesystem::path& path)
		{
			char buffer[4];
			std::ifstream in(path.string().c_str(), std::ios::binary);
			in.read(buffer, 4);
			if (strncmp(buffer, "RIFF", 4) != 0) {
				PX_ASSERT(false, "this is not a valid WAVE file");
			}

			AudioFileSpecification specs;

			in.read(buffer, 4);
			in.read(buffer, 4);      //WAVE
			in.read(buffer, 4);      //fmt
			in.read(buffer, 4);      //16
			in.read(buffer, 2);      //1
			in.read(buffer, 2);
			specs.Channels = ConvertToInt(buffer, 2);
			in.read(buffer, 4);
			specs.SampleRate = ConvertToInt(buffer, 4);
			in.read(buffer, 4);
			in.read(buffer, 2);
			in.read(buffer, 2);
			specs.Bps = ConvertToInt(buffer, 2);
			in.read(buffer, 4);      //data
			in.read(buffer, 4);
			specs.Size = ConvertToInt(buffer, 4);

			Ref<std::vector<char>> data = CreateRef<std::vector<char>>(specs.Size);
			specs.Data = data->data();

			in.read((char*)specs.Data, specs.Size);

			in.close();

			return specs;
		}


		int WavLoader::ConvertToInt(char* buffer, int len)
		{
			int result = 0;
			if (!IsBigEndian()) {
				for (int i = 0; i < len; i++)
					((char*)&result)[i] = buffer[i];
			}
			else {
				for (int i = 0; i < len; i++)
					((char*)&result)[3 - i] = buffer[i];
			}

			return result;

		}





		AudioFileSpecification OggLoader::Load(const std::filesystem::path& path)
		{
			PX_PROFILE_FUNCTION();
			short* dat;
			AudioFileSpecification specs;

			int samplesDecoded = stb_vorbis_decode_filename(path.string().c_str(), &specs.Channels, &specs.SampleRate, &dat);
			if (samplesDecoded == -1) {
				PX_CORE_ERROR("There was an error decoding samples at: %s", path.string().c_str());
				return {};
			}

			specs.Data = dat;
			// I am guessing how you retrieve the size. 2 bytes per sample as dat is a short and a short is 2 bytes typically...
			specs.Size = 2 * specs.Channels * samplesDecoded;

			return specs;
		}







		Mp3Loader::Mp3Loader()
		{
			s_mp3Loader.header[0] = 0;
		}

		AudioFileSpecification Mp3Loader::Load(const std::filesystem::path& path)
		{
			PX_PROFILE_FUNCTION();
			mp3dec_file_info_t info;
			int result = mp3dec_load(&s_mp3Loader, path.string().c_str(), &info, 0, 0);

			AudioFileSpecification specs;

			specs.Size = info.samples * sizeof(mp3d_sample_t);
			specs.Bps = specs.Size / (info.avg_bitrate_kbps * 1024.0f);
			specs.SampleRate = info.hz;
			specs.Channels = info.channels;



			specs.Data = (void*)info.buffer;
			return specs;

		}
	}

}