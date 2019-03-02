#pragma once

#include "RetroGF/RGFpch.h"
#include "RetroGF/Core.h"



// Used to log messages to the console.
// Log class is used by 'Core' and 'Client' side.


// TODO : add RGF_Debug macros around this logging system... so it can be stripped in Distribute builds

namespace RGF {

	enum class LogLevels { // The log levels
		LNone = -1,
		LTrace,
		LMsg,
		LWarn,
		LError,
		LAll
	};


	// The main Logging system
	class RGF_API Log {

		private :
			// Logger is the actual logging code that is used via function calls
			struct RGF_API Logger {
				void Trace		(const char* message, ...);
				void Message	(const char* message, ...);
				void Warn		(const char* message, ...);
				void Error		(const char* message, ...);
				void Critical	(const char* message, ...);


				void SetName(const std::string& name) { m_Name = name; }
				inline const std::string& GetName() const { return m_Name; }


				void SetLogLevel(const LogLevels& level) { m_Level = level; }
				inline const int GetLogLevel() const { return (int)m_Level; }
				private :
					std::string m_Name;
					LogLevels m_Level = LogLevels::LAll;
			};


		// Log is a container that contains two "loggers" and names them both "App" and "Client".
		public :
			static void Init() {

				s_Core->SetName("App");
				s_Client->SetName("Client");

			}

			inline static const std::shared_ptr<Log::Logger>& GetCore() { return s_Core; }
			inline static const std::shared_ptr<Log::Logger>& GetClient() { return s_Client; }

		private :
			static std::shared_ptr<Logger> s_Core;
			static std::shared_ptr<Logger> s_Client;
	};


}

// Macros accses both "Loggers" from "Log". This is used to disable and strip the "Log" code in "Distribute" builds.
#define RGF_TRACE(message, ...) RGF::Log::GetClient()->Trace(message, __VA_ARGS__ )
#define RGF_MSG(message, ...)	RGF::Log::GetClient()->Message(message, __VA_ARGS__ )
#define RGF_WARN(message, ...)	RGF::Log::GetClient()->Warn(message, __VA_ARGS__ )
#define RGF_ERROR(message, ...) RGF::Log::GetClient()->Error(message, __VA_ARGS__ )
#define RGF_CRIT(message, ...)	RGF::Log::GetClient()->Critical(message, __VA_ARGS__ )



#define RGF_CORE_TRACE(message, ...) RGF::Log::GetCore()->Trace(message, __VA_ARGS__ )
#define RGF_CORE_MSG(message, ...)	RGF::Log::GetCore()->Message(message, __VA_ARGS__ )
#define RGF_CORE_WARN(message, ...)	RGF::Log::GetCore()->Warn(message, __VA_ARGS__ )
#define RGF_CORE_ERROR(message, ...) RGF::Log::GetCore()->Error(message, __VA_ARGS__ )
#define RGF_CORE_CRIT(message, ...)	RGF::Log::GetCore()->Critical(message, __VA_ARGS__ )


