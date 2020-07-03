#pragma once


#ifdef RGF_DEBUG

	#define RGF_ENABLE_ASSERTS
#endif


#ifdef RGF_ENABLE_ASSERTS

	#define RGF_ASSERT(x, message, ...) if(!(x)){\
	RGF_CORE_CRIT(message, __VA_ARGS__ );\
	__debugbreak();\
	}\

#else
	#define RGF_ASSERT(x, message, ...)

#endif