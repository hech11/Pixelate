#include "RGFpch.h"
#include "File.h"

#include <filesystem>



namespace RGF {

	void FileIO::CreateDir(const std::string& filepath) {
		std::string dir;
		if (filepath.rfind("/") != std::string::npos) {
			dir.append(filepath.substr(0, filepath.rfind("/") + 1));
		}
		std::filesystem::create_directories(dir);
	}


	void FileIO::WriteToStorage(const std::string& filepath, const std::string& string) {

		std::fstream stream;
		if(!DoesFileExist(filepath)){
			CreateDir(filepath);
		}
		stream.open(filepath, std::fstream::out);
		stream << string;

	}

	void FileIO::MoveFileLocation(const std::string& sourceFilepath, const std::string& destinationFilepath) {
		if (!DoesFileExist(sourceFilepath)) {
			return;
		}
		if (!DoesFileExist(destinationFilepath)) {
			RGF_CORE_WARN("Creating directory.../n");
			CreateDir(destinationFilepath);
		}

		std::filesystem::copy(sourceFilepath, destinationFilepath);
		std::filesystem::remove(sourceFilepath);

	}

	void FileIO::RemoveFile(const std::string& filepath) {
		if (!DoesFileExist(filepath)) {
			return;
		}

		std::filesystem::remove(filepath);
	}


	const char* FileIO::ReadFromStorage(const std::string& filepath) {
		if (!DoesFileExist(filepath)) {
			return "File does not exist!";
		}

		std::fstream file(filepath);
		std::string line;
		std::stringstream ss;

		while (getline(file, line)) {
			ss << line;
		}

		return ss.str().c_str();
	}

	bool FileIO::DoesFileExist(const std::string& filepath) const {
		std::fstream stream(filepath);
		RGF_CORE_WARN("File at '%s' does not exist!\n", filepath.c_str());
		return !stream.fail();
	}


	unsigned int FileIO::GetFileSize(const std::string& filepath) const {
		if (!DoesFileExist(filepath)) {
			return 0;
		}
		return std::filesystem::file_size(filepath);
	}

}