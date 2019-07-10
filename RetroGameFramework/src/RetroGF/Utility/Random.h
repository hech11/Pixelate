#pragma once

#include "RetroGF/Core.h"


// This is just a thin wrapper that returns a random number.

namespace RGF {

	class RGF_API Random {
		public :
			static float GetRandomInRange(float low, float high) {
				static std::random_device m_Rd;
				std::mt19937 eng(m_Rd());
				std::uniform_real_distribution<> dist(low, high);

				return dist(eng);
			}
			static int GetRandomInRange(int low, int high) {
				static std::random_device m_Rd;
				std::mt19937 eng(m_Rd());
				std::uniform_int_distribution<> dist(low, high);

				return dist(eng);
			}
		private :
	};

}