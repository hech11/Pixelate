#pragma once


#include <string>

#include "minimp3.h"



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



	struct WavFormat : public AudioFormatSpec {
		
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


	struct MP3Format : public AudioFormatSpec {
		public :
			MP3Format();
			void LoadData(const std::string& filepath);

		private :
			static mp3dec_t mp3d;
	};

	struct OggFormat : public AudioFormatSpec {
		public:
			void LoadData(const std::string& filepath);
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