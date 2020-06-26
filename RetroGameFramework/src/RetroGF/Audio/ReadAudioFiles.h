#pragma once


// This is a temp solution to read in a .wav file.

#include <string>
#include <fstream>

#include "RetroGF/Utility/Log.h"

#include "minimp3.h"
#include "minimp3_ex.h"

namespace RGF {
	

	struct AudioFileSpec {
			enum class FileFormat {
				None = -1,
				Wav,
				Mp3,
				Ogg
			};

			int Channels, SampleRate, Bps, Size;
			FileFormat Extention;
			void* Data;

			static AudioFileSpec LoadAudioData(const std::string& filepath);

		private :
			static FileFormat DeduceFileFormat(const std::string& filepath);
	};



	struct WavFileHeader : public AudioFileSpec {
		
		public :
			void LoadData(const std::string& filepath);

		private:
			static bool IsBigEndian() {
				int a = 1;
				return !((char*)& a)[0];
			}

			static int ConvertToInt(char* buffer, int len) {
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


	struct MP3Header : public AudioFileSpec {
		public :
			MP3Header();
			void LoadData(const std::string& filepath);

		private :
			static mp3dec_t mp3d;
	};


	struct OggHeader : public AudioFileSpec {
		public:
			void LoadData(const std::string& filepath);

	};

}