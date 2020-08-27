#pragma once

#include "PXpch.h"
#include "Pixelate/Utility/File.h"



// TODO: This needs to be re factored a litte
namespace Pixelate {


	struct Session
	{
		std::string Name;
	};

	struct ProfileResult {
		std::string Name;
		long long Start, End;
		unsigned int ThreadID;
	};


	class Instrumentor {


		public :


			Instrumentor() : m_CurrentSession(nullptr), m_ProfileCount(0) 
			{

			}


			void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
	
				FileIO::Get().CreateDir(filepath);
				m_OutputStream.open(filepath);
				WriteHeader();
				m_CurrentSession = new Session{ name };
			}

			void EndSession() {
				WriteFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
				m_ProfileCount = 0;
			}

			void WriteProfile(const ProfileResult& result) {
				if (m_ProfileCount++ > 0) {
					m_OutputStream << ",";
				}

				std::string name = result.Name;
				std::replace(name.begin(), name.end(), '"', '\'');

				m_OutputStream << "{";
				m_OutputStream << "\"cat\":\"function\",";
				m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
				m_OutputStream << "\"name\":\"" << name << "\",";
				m_OutputStream << "\"ph\":\"X\",";
				m_OutputStream << "\"pid\":0,";
				m_OutputStream << "\"tid\":" << result.ThreadID << ",";
				m_OutputStream << "\"ts\":" << result.Start;
				m_OutputStream << "}";

				m_OutputStream.flush();

			}


			void WriteHeader() {
				m_OutputStream << "{\"otherData\" : {},\"traceEvents\":[";
				m_OutputStream.flush();
			}
			void WriteFooter() {
				m_OutputStream << "]}";
				m_OutputStream.flush();
			}
			
			static Instrumentor& Get() {
				static Instrumentor Instance;
				return Instance;
			}

		private :
			Session* m_CurrentSession;
			std::ofstream m_OutputStream;
			int m_ProfileCount = 0;

	};


	// NOTE: This class does not use PX's timer as PX's timer does not support micro seconds!
	class InstrumentationTimer {
		public :

			InstrumentationTimer(const std::string& name) : m_Name(name) {
				m_StartTimepoint = std::chrono::high_resolution_clock::now();
			}

			~InstrumentationTimer() {
				if (!m_Stopped) {
					Stop();
				}
			}


			void Stop() {
				auto endTimepoint = std::chrono::high_resolution_clock::now();

				long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
				long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();


				unsigned int threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
				Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

				m_Stopped = true;
			}


		private :
			std::string m_Name;
			std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
			bool m_Stopped = false;
	};


	#ifdef PX_USE_INSTRUMENTOR
		#define PX_PROFILE_BEGIN_SESSION(name, filepath) ::Pixelate::Instrumentor::Get().BeginSession(name, filepath);
		#define PX_PROFILE_END_SESSION(name, filepath) ::Pixelate::Instrumentor::Get().EndSession();
		#define PX_PROFILE_SCOPE(name) ::Pixelate::InstrumentationTimer timer##__LINE__(name);
		#define PX_PROFILE_FUNCTION() PX_PROFILE_SCOPE(__FUNCSIG__)

	#else
		#define PX_PROFILE_BEGIN_SESSION(name, filepath)
		#define PX_PROFILE_END_SESSION(name, filepath)
		#define PX_PROFILE_SCOPE(name)
		#define PX_PROFILE_FUNCTION()

	#endif


}