#pragma once



// Common macros that are used when developing RGF.

#include <memory>

#ifdef RGF_PLATFORM_WINDOWS // TODO: I changed Retro into a static lib. Maybe remove this soon?

	#ifdef RGF_BUILD_DLL
		#define RGF_API 
	#else
		#define RGF_API 
	#endif
#else
	#error RetroGameFramework only supports windows!
#endif


namespace RGF {

	template<typename T>
	using Scoped = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scoped<T> CreateScoped(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}


	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using WeakRef = std::weak_ptr<T>;
}