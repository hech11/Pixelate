#pragma once


#ifdef PX_DEBUG

	#define PX_ENABLE_ASSERTS
#endif


#ifdef PX_ENABLE_ASSERTS

	#define PX_ASSERT(x, message, ...) if(!(x)){\
	PX_CORE_CRIT(message, __VA_ARGS__ );\
	__debugbreak();\
	}\

#else
	#define PX_ASSERT(x, message, ...)

#endif