#include "PXpch.h"
#include "File.h"

#include <filesystem>

#include "Pixelate/Core/Log.h"


namespace Pixelate {

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
			PX_CORE_WARN("Creating directory.../n");
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


	std::string FileIO::ReadFromStorage(const std::string& filepath) {
		std::string line;

		if (!DoesFileExist(filepath)) {
			PX_CORE_ERROR("File at \"%s\" does not exist!", filepath.c_str());
			line = "Error";
			return line;
		}

		std::ifstream file(filepath, std::ios::binary);
		std::stringstream ss;

		while (getline(file, line)) {
			ss << line << '\n';
		}


		return ss.str();
	}

	bool FileIO::DoesFileExist(const std::string& filepath) const {
		std::ifstream stream(filepath, std::ios::binary);
		return !stream.fail();
	}


	unsigned int FileIO::GetFileSize(const std::string& filepath) const {
		if (!DoesFileExist(filepath)) {
			return 0;
		}
		return std::filesystem::file_size(filepath);
	}

}