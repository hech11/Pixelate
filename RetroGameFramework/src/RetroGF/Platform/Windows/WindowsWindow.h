#pragma once

#include "RetroGF/Core.h"
#include "RGFpch.h"

#include "RetroGF/WindowImpl.h"

#include <GLFW/glfw3.h>


// This is the implementation of the "WindowsImpl.h" pure virtual functions.
// Other platforms will inheret "WindowsImpl" and implement their own pure virtual functions.




/*
	TODO: This implementation of "WindowsImpl" uses GLFW for creating the window.
	At some point I may switch to "Windows.h" instead to create a window and handle events.
*/

 // TODO: "SetVsync" is the only "attribute" function that is not defined here. I should look into this.


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

			inline const std::string& GetTitle() const { return m_Data.props.Title; };
			inline bool IsVsyncEnabled() const { return m_Data.Vsync; };
			inline int GetWidth() const { return m_Data.props.Width; };
			inline int GetHeight() const { return m_Data.props.Height; };

		private :
			GLFWwindow* m_Window;



			struct WindowData { // Window data that will be passed into glfw.
			
				WindowProps props;
				bool Vsync;
			};


			/*
				"m_Data" should create events callbacks, dispatch events and create the window. 
				Right now "m_Data" is used to create the window. This is because 
				an event callback function has not been implemented in "Application" yet.
			*/
			WindowData m_Data;
	};
}