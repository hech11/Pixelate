#pragma once

// To test mini audio

#include <string>

namespace RGF {

	class Audio {


		public :
			static void Init(const std::string& filepath);
			static void Play();
			static void Shutdown();


	};
}