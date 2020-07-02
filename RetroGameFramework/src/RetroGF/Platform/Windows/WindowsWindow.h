#pragma once

#include "RetroGF/Core/Core.h"
#include "RGFpch.h"

#include "RetroGF/Core/Window.h"



// This is the implementation of the "WindowsImpl.h" pure virtual functions.
// Other platforms will inheret "Windows" and implement their own pure virtual functions.




/*
	TODO: This implementation of "WindowsWindow" uses GLFW for creating the window.
	At some point I may switch to <Windows.h> instead to create a window and handle events.
*/


struct GLFWwindow;
namespace RGF {

	class RGF_API WindowsWindow : public WindowImpl {
		public :
			WindowsWindow(const WindowProps& props);
			virtual ~WindowsWindow();

			void StartUp(const WindowProps& props) override;
			void ShutDown() override;

			void OnUpdate() override;


			void SetVsync(bool enabled) override;
			void SetTitle(const std::string& title) override;

			inline void SetEventCallback(const EventCallbackFncPtr& callback) override { m_Data.EventCallback = callback; }
			inline const std::string& GetTitle() const override { return m_Data.props.Title; };
			inline bool IsVsyncEnabled() const override { return m_Data.Vsync; };
			inline int GetWidth() const override { return m_Data.props.Width; };
			inline int GetHeight() const override { return m_Data.props.Height; };
			inline unsigned int GetXPos() const override { return m_Data.props.xPos; };
			inline unsigned int GetYPos() const override { return m_Data.props.yPos; };

			inline void* GetNativeWindow() const override { return m_Window; }
			inline RGF::RenderingContext* GetContext() const override { return m_Context; }


		private :
			GLFWwindow* m_Window;

			// "m_Data" should create events callbacks from GLFW and create the window. 
			WindowData m_Data;
	};

}