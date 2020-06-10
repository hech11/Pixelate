#pragma once


// This is a temp solution to read in a .wav file.

#include <string>
#include <fstream>

namespace RGF {

	struct WavFileHeader {
	
		private :
			static bool IsBigEndian() {
				int a = 1;
				return !((char*)&a)[0];
			}

			static int ConvertToInt(char* buffer, int len) {
				int a = 0;
				if (!IsBigEndian())
					for (int i = 0; i < len; i++)
						((char*)&a)[i] = buffer[i];
				else
					for (int i = 0; i < len; i++)
						((char*)&a)[3 - i] = buffer[i];
				return a;
			}
			
		public :
			// This is unsafe code.
			static void* LoadData(const std::string& filepath, int* channels, int* sampleRate, int* bps, int* size) {

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
				*channels = ConvertToInt(buffer, 2);
				in.read(buffer, 4);
				*sampleRate = ConvertToInt(buffer, 4);
				in.read(buffer, 4);
				in.read(buffer, 2);
				in.read(buffer, 2);
				*bps = ConvertToInt(buffer, 2);
				in.read(buffer, 4);      //data
				in.read(buffer, 4);
				*size = ConvertToInt(buffer, 4);
				char* data = (char*)alloca(*size);
				in.read(data, *size);
				return data;

		}

	};

}