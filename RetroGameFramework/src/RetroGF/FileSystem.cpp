#include "RGFpch.h"
#include "FileSystem.h"

#include <filesystem>



namespace RGF {
	FileSystem* FileSystem::s_Instance = nullptr;


	// TODO: instanciating and init'ing in the entry point may be not be needed.
	FileSystem::FileSystem() { 
		if (s_Instance != nullptr) {
			RGF_CORE_WARN("File System already created!");
			return;
		}
		s_Instance = this;
	}

	void FileSystem::WriteFile(const std::string& filepath, const File& file) {

		std::fstream stream;
		if(!DoesFileExist(filepath)){
			std::string dir;
			if (filepath.rfind("/") != std::string::npos) {
				dir.append(filepath.substr(0, filepath.rfind("/") + 1));
			}
			std::filesystem::create_directories(dir);
		}
		stream.open(filepath, std::fstream::out);
		stream << file.GetData();

	}
	std::string FileSystem::ReadFile(const std::string& filepath) {
		if (!DoesFileExist(filepath)) {
			return "File not exist!";
		}

		std::fstream file(filepath);
		std::string line;
		std::stringstream ss;

		while (getline(file, line)) {
			ss << line;
		}

		return ss.str();
	}
	bool FileSystem::DoesFileExist(const std::string& filepath) {
		std::fstream stream(filepath);
		return !stream.fail();
	}

}