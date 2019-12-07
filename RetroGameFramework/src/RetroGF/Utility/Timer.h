#pragma once

#include "RetroGF/Core/Core.h"

#include "RGFpch.h"


// Timer is a stopwatch that keeps running when instantiated.

// TODO : Instead of using Chrono.h use the platform header files timer instead.
namespace RGF {

	class RGF_API Timer {

		public :
			Timer() { Reset(); }
			~Timer(){}

			void Reset(){ m_Timer = std::chrono::high_resolution_clock::now(); }


			inline float GetElapsedMillis() const {
				const auto& current = std::chrono::high_resolution_clock::now();
				const auto& cycles = std::chrono::duration<float>(std::chrono::duration_cast<std::chrono::milliseconds>(current - m_Timer));
				return cycles.count() * 1000;

			}

			inline float GetElapsedSeconds() const {
				const auto& current = std::chrono::high_resolution_clock::now();
				const auto& cycles = std::chrono::duration<float>(std::chrono::duration_cast<std::chrono::milliseconds>(current - m_Timer));
				return cycles.count();
			}

		private :
			std::chrono::time_point<std::chrono::steady_clock> m_Timer;


	};


	

}