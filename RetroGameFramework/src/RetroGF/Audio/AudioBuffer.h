#pragma once


#include <string>

#include "minimp3.h"

#include "RetroGF/Debug/Instrumentor.h"


namespace RGF {



		
	struct AudioFormatSpec {
			enum class FileFormat {
				None = -1,
				Wav,
				Mp3,
				Ogg
			};

			int Channels, SampleRate, Bps, Size;
			FileFormat Extention;
			void* Data;

			static AudioFormatSpec LoadAudioData(const std::string& filepath);

		private :
			static FileFormat DeduceFileFormat(const std::string& filepath);
	};



	struct WavFormat {
		
		public :
			static void LoadData(const std::string& filepath, AudioFormatSpec* specs);
		private:
			static bool IsBigEndian() {
				RGF_PROFILE_FUNCTION();
				int a = 1;
				return !((char*)& a)[0];
			}
			static int ConvertToInt(char* buffer, int len) {
				RGF_PROFILE_FUNCTION();
				int a = 0;
				if (!IsBigEndian())
					for (int i = 0; i < len; i++)
						((char*)& a)[i] = buffer[i];
				else
					for (int i = 0; i < len; i++)
						((char*)& a)[3 - i] = buffer[i];
				return a;
			}

	};


	struct MP3Format {
		public :
			MP3Format();

			static void LoadData(const std::string& filepath, AudioFormatSpec* specs);

		private :
			static mp3dec_t mp3d;
	};

	struct OggFormat {
		public:
			static void LoadData(const std::string& filepath, AudioFormatSpec* specs);
	};




	class AudioBuffer {

		public:

			AudioBuffer(const AudioFormatSpec& spec);
			~AudioBuffer();

			void SetData(const AudioFormatSpec& spec);

			unsigned int GetHandleID() const { return m_AudioID; }
		private:
			unsigned int m_AudioID;
	};
}