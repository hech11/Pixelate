#pragma once


#include "Pixelate/Core/Core.h"
#include "PXpch.h"

#include "Pixelate/Events/Event.h"

#include "Pixelate/Rendering/RenderingContext.h"
/*  
	This is a representation of a window.
	It is only a interface containing pure virtual functions.

	The pure virutal functions are to be inherented and defined in a platform specific window.
	( see "Platform/Windows/WindowsWindow.h" for more info )

	PX only supports windows at this moment, "WindowsWindow".
	When / If PX supports other platforms, new files will be created for 
	that specific platform. eg "MacWindows" will implement a Mac window.
 */



namespace Pixelate {


	// Contains all the properties that the window will contain.
	struct PX_API WindowProps {
		unsigned int Width, Height, xPos, yPos;
		std::string Title;

		WindowProps(unsigned int width = 1280, unsigned int height = 720,
			unsigned int xpos = 50, unsigned int ypos = 50,
			const std::string& title = "Pixelate")
			: Width(width), Height(height), xPos(xpos), yPos(ypos), Title(title)
		{}
	};


	class PX_API Window {

		public :


			// function pointer used to create a callback events. From window to "Application".
			using EventCallbackFncPtr = std::function<void(Event&)>;


			struct WindowData { // Window data that will be passed into glfw.

				WindowProps props;
				bool Vsync;

				// Implement the callback using "SetEventCallback". Declared in "WindowImpl".
				EventCallbackFncPtr EventCallback;
			};

			virtual ~Window(){}

			virtual void StartUp(const WindowProps& props = WindowProps()) = 0;
			virtual void ShutDown() = 0;

			virtual void OnUpdate() = 0;

			// Window attributes
			virtual void SetVsync(bool enabled) = 0;
			virtual void SetTitle(const std::string& title) = 0;
						
			virtual inline void SetEventCallback(const EventCallbackFncPtr& callback) = 0;
			virtual inline int GetWidth() const = 0;
			virtual inline int GetHeight() const = 0;
			virtual inline const std::string& GetTitle() const = 0;
			virtual inline bool IsVsyncEnabled() const = 0;

			virtual unsigned int GetXPos() const = 0;
			virtual unsigned int GetYPos() const = 0;


			virtual inline void* GetNativeWindow() const = 0;
			virtual inline Pixelate::RenderingContext* GetContext() const = 0;

			// To be defined in the inherented DERIVED class.
			static Window* Create(const WindowProps& props = WindowProps()); // Should return a HEAP allocated window object.

	};

}