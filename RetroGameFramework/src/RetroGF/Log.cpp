#include "RGFpch.h"
#include "Log.h"



namespace RGF {

	// Used to change the colour of the text in the console. Windows only.
	#ifdef RGF_PLATFORM_WINDOWS

		enum COLOR_ID : unsigned char {
			GREEN = 10,
			RED = 12,
			YELLOW = 14,
			WHITE = 15
		};

		#define CHANGE_CONSOLE_COLOR(colorID) SetConsoleTextAttribute(s_Handle, colorID)
		static HANDLE s_Handle = GetStdHandle(STD_OUTPUT_HANDLE);


	#endif



	// Static var's here.
	std::shared_ptr<Log::Logger> Log::s_Core = std::make_shared<Log::Logger>();
	std::shared_ptr<Log::Logger> Log::s_Client = std::make_shared<Log::Logger>();




	// The code that actually logs the message and it's args to the console.
	// This is used in the "Logger" s definitions.
	#define PRINT_MSG(FuncName, SysTime, message, ...) va_list args;\
											  va_start(args, message);\
											  printf("[%s][%s][%s]:\t", SysTime.c_str(), m_Name.c_str(), FuncName);\
											  vprintf(message, args)




	/* 
		TODO: this may be unsafe and there may be a better way to implement this...
			  may want to change this soon.
	*/
	static const std::string GetSystemTime() { // This function is created because the "logger" will print the system's time.

		std::time_t time = std::time(0);
		std::tm* now = std::localtime(&time);

		std::stringstream ss;
		ss << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec;
		return ss.str();
	}



	// Logger's function implementations.

	void Log::Logger::Trace(const char* message, ...) {

		if ((m_Level != LogLevels::LAll && m_Level != LogLevels::LTrace) || m_Level == LogLevels::LNone)
			return;
		CHANGE_CONSOLE_COLOR(COLOR_ID::WHITE);
		PRINT_MSG("Trace", GetSystemTime(), message);
	}
	void Log::Logger::Message(const char* message, ...) {

		if ((m_Level != LogLevels::LAll && m_Level != LogLevels::LMsg) || m_Level == LogLevels::LNone)
			return;
		CHANGE_CONSOLE_COLOR(COLOR_ID::GREEN);
		PRINT_MSG("Msg", GetSystemTime(), message);

	}
	void Log::Logger::Warn(const char* message, ...) { 

		if ((m_Level != LogLevels::LAll && m_Level != LogLevels::LWarn) || m_Level == LogLevels::LNone)
			return;
		CHANGE_CONSOLE_COLOR(COLOR_ID::YELLOW);
		PRINT_MSG("Warn", GetSystemTime(), message);
	}
	void Log::Logger::Error(const char* message, ...) {

		if ((m_Level != LogLevels::LAll && m_Level != LogLevels::LError) || m_Level == LogLevels::LNone)
			return;
		CHANGE_CONSOLE_COLOR(COLOR_ID::RED);
		PRINT_MSG("Error", GetSystemTime(), message);
	}
	void Log::Logger::Critical(const char* message, ...) {

		CHANGE_CONSOLE_COLOR(COLOR_ID::RED);
		PRINT_MSG("Crit", GetSystemTime(), message);
	}

}