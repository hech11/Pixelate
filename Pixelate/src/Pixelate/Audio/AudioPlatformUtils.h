#pragma once



namespace Pixelate {

	class AudioPlatformUtils {

		public :
			static void Init();
			static void Shutdown();

			static float GetPeakValue();

	};

}