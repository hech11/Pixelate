#pragma once

#include "RetroGF/Core.h"

#include "RGFpch.h"


namespace RGF {
	// TODO : Instead of using Chrono.h use the platform header files timer instead.

	class RGF_API Timer {

		public :
			Timer() { Reset(); }
			~Timer(){}

			void Reset(){ m_Timer = std::chrono::high_resolution_clock::now(); }
			inline float GetElapsedMillis() const {
				const auto& current = std::chrono::high_resolution_clock::now();
				const auto& cycles = std::chrono::duration<float>(std::chrono::duration_cast<std::chrono::milliseconds>(current - m_Timer));
				return cycles.count();

			}

			inline float GetElapsedSeconds() const {
				const auto& current = std::chrono::high_resolution_clock::now();
				const auto& cycles = std::chrono::duration<float>(std::chrono::duration_cast<std::chrono::milliseconds>(current - m_Timer));
				return cycles.count() * 1000;
			}

		private :
			std::chrono::time_point<std::chrono::steady_clock> m_Timer;


	};

}