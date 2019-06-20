#pragma once

#include "RetroGF/Core.h"


// This is just a thin wrapper that returns a random number.

namespace RGF {

	class RGF_API Random {
		public :
			static float GetRandomInRange(float low, float high) {
				std::mt19937 eng(m_Rd());
				std::uniform_real_distribution<> dist(low, high);

				return dist(eng);
			}
			static int GetRandomInRange(int low, int high) {
				std::mt19937 eng(m_Rd());
				std::uniform_int_distribution<> dist(low, high);

				return dist(eng);
			}
		private :
			static std::random_device m_Rd;
	};

}