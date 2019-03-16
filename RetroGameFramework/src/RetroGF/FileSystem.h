#pragma once

#include "RGFpch.h"

namespace RGF {

	class RGF_API File {
		public :

			File(const char* data) {
				m_Size = strlen(data);
				m_Data = new char[m_Size + 1];
				memcpy(m_Data, data, m_Size);
				m_Data[m_Size] = 0;
			}
			~File() {
				delete[] m_Data;
			}

			File(const File& other)
				: m_Size(other.m_Size) {
				m_Data = new char[m_Size + 1];
				memcpy(m_Data, other.m_Data, m_Size + 1);
				m_Data[m_Size] = 0;

			}

			char* GetData() const { return m_Data; }

		private :
			char* m_Data;
			unsigned int m_Size;

	};


	class RGF_API FileSystem {
		private :
			static FileSystem* m_Instance;
		public :
			FileSystem();

			static void WriteFile(const std::string& filepath, const File& file);
			static std::string ReadFile(const std::string& filepath);
			static bool DoesFileExist(const std::string& filepath);
	};
}