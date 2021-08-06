#include "PXpch.h"

#include "EditorConsoleLogger.h"

#include <imgui.h>

namespace Pixelate {



	struct ConsoleLogs {

		ImGuiTextBuffer TextBuffer;
		uint8_t Color;

		ConsoleLogs(const ImGuiTextBuffer& buffer, int color) : TextBuffer(buffer), Color(color){}

	};

	static std::vector<ConsoleLogs> s_Logs;

	 

	static void TraceCallback(const char* id, const std::string& time, const char* msg, const char* name, va_list args) IM_FMTARGS(2) {

		ImGuiTextBuffer temp;
		std::stringstream ss;
		ss << "[" << time << "][" << name << "][" << id << "]:\t";
		temp.append(ss.str().c_str());
		temp.appendfv(msg, args);

		s_Logs.emplace_back(temp, 0);
	}

	static void MessageCallback(const char* id, const std::string& time, const char* msg, const char* name, va_list args)IM_FMTARGS(2) {
		
		ImGuiTextBuffer temp;
		std::stringstream ss;
		ss << "[" << time << "][" << name << "][" << id << "]:\t";
		temp.append(ss.str().c_str());
		temp.appendfv(msg, args);

		s_Logs.emplace_back( temp, 1);
	}

	static void WarnCallback(const char* id, const std::string& time, const char* msg, const char* name, va_list args)IM_FMTARGS(2) {
		ImGuiTextBuffer temp;
		std::stringstream ss;
		ss << "[" << time << "][" << name << "][" << id << "]:\t";
		temp.append(ss.str().c_str());
		temp.appendfv(msg, args);

		s_Logs.emplace_back(temp, 2);


	}


	static void ErrorCallback(const char* id, const std::string& time, const char* msg, const char* name, va_list args) IM_FMTARGS(2) {

		ImGuiTextBuffer temp;
		std::stringstream ss;
		ss << "[" << time << "][" << name << "][" << id << "]:\t";
		temp.append(ss.str().c_str());
		temp.appendfv(msg, args);

		s_Logs.emplace_back(temp, 3);

	}

	static void CritCallback(const char* id, const std::string& time, const char* msg, const char* name, va_list args) IM_FMTARGS(2) {
		ImGuiTextBuffer temp;
		std::stringstream ss;
		ss << "[" << time << "][" << name << "][" << id << "]:\t";
		temp.append(ss.str().c_str());
		temp.appendfv(msg, args);

		s_Logs.emplace_back(temp, 3);

	}

	EditorConsoleLogger::EditorConsoleLogger(bool registerCallbacks)
	{
		s_Logs.reserve(256);
		m_Callbacks.Trace = TraceCallback;
		m_Callbacks.Message = MessageCallback;
		m_Callbacks.Warn = WarnCallback;
		m_Callbacks.Error = ErrorCallback;
		m_Callbacks.Critical = CritCallback;

		Log::RegisterUserCallbacks(m_Callbacks);
	}

	void EditorConsoleLogger::OnImguiRender() {

		if (!m_IsPanelOpen)
			return;

		ImGui::Begin("Console", &m_IsPanelOpen);
		if (ImGui::Button("Clear")) {
			s_Logs.clear();
		};
		ImGui::SameLine();

		static bool lTrace = true;
		static bool lMsg = true;
		static bool lWarn = true;
		static bool lError = true;

		uint8_t lvl = 0;

		ImGui::Checkbox("AutoScroll", &m_AutoScroll);
		ImGui::Separator();

		ImGui::Checkbox("Log Trace", &lTrace);
		ImGui::SameLine();
		ImGui::Checkbox("Log Messages", &lMsg);
		ImGui::SameLine();
		ImGui::Checkbox("Log Warnings", &lWarn);
		ImGui::SameLine();
		ImGui::Checkbox("Log Errors", &lError);
		ImGui::SameLine();
		
		if (lTrace)
			lvl |= LogLevels::LTrace;

		if (lMsg)
			lvl |= LogLevels::LMsg;

		if (lWarn)
			lvl |= LogLevels::LWarn;

		if (lError)
			lvl |= LogLevels::LError;

		PX_CORE_LOG_LEVEL((LogLevels)lvl);
		PX_LOG_LEVEL((LogLevels)lvl);


		ImGui::NewLine();
		ImGui::Separator();
		ImGui::BeginChild(1);
		ImVec4 txtCol;
		for (int i = 0; i < s_Logs.size(); i++) {
			if (s_Logs[i].Color == 1) {
				txtCol = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else if (s_Logs[i].Color == 0) {
				txtCol = ImVec4(0.2f, 0.8f, 0.2f, 1.0f);
			} else if (s_Logs[i].Color == 2) {
				txtCol = ImVec4(0.8f, 0.8f, 0.1f, 1.0f);
			}
			else if (s_Logs[i].Color == 3) {
				txtCol = ImVec4(0.8f, 0.4f, 0.2f, 1.0f);
			}

			ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, txtCol);
			ImGui::TextUnformatted(s_Logs[i].TextBuffer.begin());
			ImGui::PopStyleColor();

		}
		if(m_AutoScroll)
			ImGui::SetScrollHere(1.0f);
		ImGui::EndChild();
		ImGui::End();

	}

}