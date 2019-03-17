#include "Random.h"

namespace RGF {


	float Random::GetRandomInRange(float low, float high) {
		std::random_device m_Rd;
		std::mt19937 eng(m_Rd());
		std::uniform_real_distribution<> dist(low, high);

		return dist(eng);
	}
	int Random::GetRandomInRange(int low, int high) {
		std::random_device m_Rd;
		std::mt19937 eng(m_Rd());
		std::uniform_int_distribution<> dist(low, high);

		return dist(eng);
	}

}