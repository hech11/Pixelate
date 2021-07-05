#pragma once



// Common macros that are used when developing PX.

#include <memory>
#include <iostream>
#include <string>



#ifdef PX_PLATFORM_WINDOWS // TODO: I changed Retro into a static lib. Maybe remove this soon?

	#ifdef PX_BUILD_DLL
		#define PX_API 
	#else
		#define PX_API 
	#endif
#else
	#error Pixelate only supports windows!
#endif


#define PX_BIND_EVENT_FNC(x) std::bind(&x, this, std::placeholders::_1)


#include "Assert.h"
namespace Pixelate {

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

	void InitEngine();
	void ShutdownEngine();

}

#define BIT(x) (1 << x)
#define STRINGIFY(x) #x


// TODO: move this when i decide to create a custom memory allocator
#ifdef PX_USE_CUSTOM_MEM_ALLOC

	inline void* operator new(size_t size) {
		return malloc(size);
	}
	
	
	inline void* operator new(size_t size, const char* file, unsigned int line) {
		void* address = malloc(size);

		std::string f = file;
		std::string fileTrip = f.substr(f.rfind('\\'), f.length());
		std::cout << "Allocated " << size << " byte(s) at address "
			<< address << " in " << fileTrip << ":" << line << std::endl;
		return address;
	}
	
	#define new new(__FILE__, __LINE__)

#endif