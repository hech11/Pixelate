#pragma once


#include <fstream>
#include <unordered_map>

#include "Pixelate/Core/Core.h"



namespace Pixelate {

	enum class AudioFormat {
		None = -1,
		Wav,
		Mp3,
		Ogg
	};


	struct AudioFileSpecification {
		AudioFormat Format;
		int Channels, SampleRate, Bps, Size;
		void* Data;
	};

	class AudioLoader {
		public :
			static AudioFileSpecification Load(const std::filesystem::path& path);
	};

	namespace Utils {
		class AudioBaseLoader {
			public:
				virtual AudioFileSpecification Load(const std::filesystem::path& path) = 0;
		};

		class WavLoader : public AudioBaseLoader {
			public :
				AudioFileSpecification Load(const std::filesystem::path& path) override;

			private :
				static int ConvertToInt(char* buffer, int len);
		};


		class OggLoader : public AudioBaseLoader {
			public:
				AudioFileSpecification Load(const std::filesystem::path& path) override;
		};

		class Mp3Loader : public AudioBaseLoader {

			public:
				Mp3Loader();
				AudioFileSpecification Load(const std::filesystem::path& path) override;
		};
	}
}