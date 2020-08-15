#pragma once


#include "PXpch.h"
#include "Pixelate/Core/Core.h"

#ifndef PX_DISTRIBUTE


// Used to log messages to the console.
// Log class is used by 'Core' and 'Client' side.


// TODO : add PX_Debug macros around this logging system... so it can be stripped in Distribute builds.
/*
   TODO : Investigate... do the two "Loggers" in "Log" really need to be shared pointers? 
		  ( they could be a unique ptr or stack allocated instead )
*/

namespace Pixelate {


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
	class Log {

		// The "Logger" struct contains the actual logging code that is used.
		struct Logger {
			void Trace(const char* message, ...);
			void Message(const char* message, ...);
			void Warn(const char* message, ...);
			void Error(const char* message, ...);
			void Critical(const char* message, ...);


			void SetName(const std::string& name) { m_Name = name; }
			inline const std::string& GetName() const { return m_Name; }


			void SetLogLevel(const LogLevels& level) { m_Level = level; }
			inline const int GetLogLevel() const { return (int)m_Level; }
		private:
			std::string m_Name;
			LogLevels m_Level = LogLevels::LAll;
		};


		// Log is a container that contains two "Loggers" and names them both "App" and "Client".
		// Log is initialized in "EntryPoint.h".
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

/*
	Macros accses both "Loggers" from the "Log" class.
	These macros are created to quickly log messages to the console without typing the long signature of each loggers.
	These macros are also used to disable and strip the "Log" and "Logger" code in "Distribute" builds.
*/


#define PX_TRACE(message, ...) Pixelate::Log::GetClient()->Trace(message, __VA_ARGS__ )
#define PX_MSG(message, ...)	Pixelate::Log::GetClient()->Message(message, __VA_ARGS__ )
#define PX_WARN(message, ...)	Pixelate::Log::GetClient()->Warn(message, __VA_ARGS__ )
#define PX_ERROR(message, ...) Pixelate::Log::GetClient()->Error(message, __VA_ARGS__ )
#define PX_CRIT(message, ...)	Pixelate::Log::GetClient()->Critical(message, __VA_ARGS__ )


#define PX_CORE_TRACE(message, ...) Pixelate::Log::GetCore()->Trace(message, __VA_ARGS__ )
#define PX_CORE_MSG(message, ...)	Pixelate::Log::GetCore()->Message(message, __VA_ARGS__ )
#define PX_CORE_WARN(message, ...)	Pixelate::Log::GetCore()->Warn(message, __VA_ARGS__ )
#define PX_CORE_ERROR(message, ...) Pixelate::Log::GetCore()->Error(message, __VA_ARGS__ )
#define PX_CORE_CRIT(message, ...)	Pixelate::Log::GetCore()->Critical(message, __VA_ARGS__ )




#else 

#define PX_TRACE(message, ...) 
#define PX_MSG(message, ...) 
#define PX_WARN(message, ...) 
#define PX_ERROR(message, ...) 
#define PX_CRIT(message, ...) 


#define PX_CORE_TRACE(message, ...) 
#define PX_CORE_MSG(message, ...) 
#define PX_CORE_WARN(message, ...)
#define PX_CORE_ERROR(message, ...) 
#define PX_CORE_CRIT(message, ...)



#endif
