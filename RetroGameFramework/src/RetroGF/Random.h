#pragma once

#include "RGFpch.h"

namespace RGF {

	class Random {
		public :
			static float GetRandomInRange(float low, float high);
			static int GetRandomInRange(int low, int high);

	};

}