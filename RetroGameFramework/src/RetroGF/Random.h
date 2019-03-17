#pragma once

#include "RetroGF/Core.h"

namespace RGF {

	class RGF_API Random {
		public :
			static float GetRandomInRange(float low, float high);
			static int GetRandomInRange(int low, int high);

	};

}