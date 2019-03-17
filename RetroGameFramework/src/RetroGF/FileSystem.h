#pragma once

#include "RGFpch.h"

namespace RGF {

	class RGF_API File {
		public :

			File(const char* data, std::string debugName = "File") : m_DebugName(debugName) {
				WriteData(data);
			}
			~File() {
				ClearData();
			}

			File(const File& other)
				: m_Size(other.m_Size), m_DebugName(other.m_DebugName) {
				m_Data = new char[m_Size + 1];
				memcpy(m_Data, other.m_Data, m_Size + 1);
				m_Data[m_Size] = 0;

			}

			inline char* GetData() const { return m_Data; }

			void ClearData() {
				HasData = false;
				delete[] m_Data;
			}

			void WriteData(const char* data) {
				if (HasData) {
					RGF_CORE_WARN("'%s' already has data loaded in memory! The data was not overridden!", m_DebugName.c_str());
					return;
				}
				m_Size = strlen(data);
				m_Data = new char[m_Size + 1];
				memcpy(m_Data, m_Data, m_Size + 1);
				m_Data[m_Size] = 0;

				HasData = true;
			}
			inline const std::string& GetName() const { return m_DebugName; }
			void SetName(const std::string& name) { m_DebugName = name; }

		private : // debugging AND saftey variables.
			std::string m_DebugName;
			bool HasData = false;
		private :
			char* m_Data;
			unsigned int m_Size;

	};


	class RGF_API FileSystem {
		private :
			static FileSystem* s_Instance;
		public :
			FileSystem();

			static void WriteFile(const std::string& filepath, const File& file);
			static std::string ReadFile(const std::string& filepath);
			static bool DoesFileExist(const std::string& filepath);
	};
}