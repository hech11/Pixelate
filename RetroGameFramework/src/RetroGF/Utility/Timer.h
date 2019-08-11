#pragma once

#include "RetroGF/Core.h"

#include "RGFpch.h"


// Timer is a stopwatch that keeps running when instanciated.

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


	// This is used in althgorithms. It can give an idea on how much time an althgorithm takes to be executed.
	class RGF_API DebugStopwatch {
		public :
			DebugStopwatch() {
				m_Timer.Reset();
			}
			~DebugStopwatch() {
				float time = m_Timer.GetElapsedMillis();
				RGF_CORE_MSG("[DebugStopwatch]: Took '%s' milliseconds to complete althgorithm!", time);
			}
		private :
			Timer m_Timer;
	};

}