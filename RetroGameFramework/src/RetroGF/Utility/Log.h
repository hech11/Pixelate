#pragma once


#include "RGFpch.h"
#include "RetroGF/Core.h"

#ifndef RGF_DISTRIBUTE


// Used to log messages to the console.
// Log class is used by 'Core' and 'Client' side.


// TODO : add RGF_Debug macros around this logging system... so it can be stripped in Distribute builds.
/*
   TODO : Invesigate... do the two "Loggers" in "Log" really need to be shared pointers? 
		  ( they could be a unique ptr or stack allocated instead )
*/

namespace RGF {


	// The log levels.
	enum class LogLevels {
		LNone = -1,
		LTrace,
		LMsg,
		LWarn,
		LError,
		LAll
	};


	// The main Logging system.
	class RGF_API Log {
		struct Logger;

		// Log is a container that contains two "Loggers" and names them both "App" and "Client".
		// Log is initialised in "EntryPoint.h".
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



			// The "Logger" struct contains the actual logging code that is used.
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
	};


}

/*
	Macros accses both "Loggers" from the "Log" class.
	These macros are created to quickly log messages to the console without typing the long signature of each loggers.
	These macros are also used to disable and strip the "Log" and "Logger" code in "Distribute" builds.
*/


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


#define RGF_ASSERT(x, message, ...) if(!(x)){\
RGF::Log::GetCore()->Critical(message, __VA_ARGS__ );\
__debugbreak();\
}\

#else 

#define RGF_TRACE(message, ...) 
#define RGF_MSG(message, ...) 
#define RGF_WARN(message, ...) 
#define RGF_ERROR(message, ...) 
#define RGF_CRIT(message, ...) 


#define RGF_CORE_TRACE(message, ...) 
#define RGF_CORE_MSG(message, ...) 
#define RGF_CORE_WARN(message, ...)
#define RGF_CORE_ERROR(message, ...) 
#define RGF_CORE_CRIT(message, ...)

#define RGF_ASSERT(x, message, ...) if(!(x)){\
__debugbreak();\
}\



#endif
