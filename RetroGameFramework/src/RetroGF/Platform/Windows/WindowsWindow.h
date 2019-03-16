#pragma once

#include "RetroGF/Core.h"
#include "RGFpch.h"

#include "RetroGF/WindowImpl.h"



// This is the implementation of the "WindowsImpl.h" pure virtual functions.
// Other platforms will inheret "WindowsImpl" and implement their own pure virtual functions.




/*
	TODO: This implementation of "WindowsImpl" uses GLFW for creating the window.
	At some point I may switch to "Windows.h" instead to create a window and handle events.
*/

 // TODO: "SetVsync" is the only "attribute" function that is not defined here. I should look into this.


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
			void SetTitle(const std::string& title) override { m_Data.props.Title = title; }

			inline void SetEventCallback(const EventCallbackFncPtr& callback) override { m_Data.EventCallback = callback; }
			inline const std::string& GetTitle() const override { return m_Data.props.Title; };
			inline bool IsVsyncEnabled() const override { return m_Data.Vsync; };
			inline int GetWidth() const override { return m_Data.props.Width; };
			inline int GetHeight() const override { return m_Data.props.Height; };
			inline unsigned int GetXPos() const override { return m_Data.props.xPos; };
			inline unsigned int GetYPos() const override { return m_Data.props.yPos; };

			inline void* GetNativeWindow() const override { return m_Window; }


		private :
			GLFWwindow* m_Window;



			struct WindowData { // Window data that will be passed into glfw.
			
				WindowProps props;
				bool Vsync;

				// Implement the callback using "SetEventCallback". Declared in "WindowImpl".
				EventCallbackFncPtr EventCallback;
			};


			// "m_Data" should create events callbacks from GLFW and create the window. 
			WindowData m_Data;
	};

}