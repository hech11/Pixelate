#pragma once

#include "RGFpch.h"

// This file system will load & create a binary file.


// This uses the c++ way to load files. Maybe in the future: switch between c and c++ and benchmark.

/*
	TODO: This is also missing features. This need to be redone. Features such as:

	- Read files only as a binary format.
	- Switch between read files as binary or read as text.
	- Make read-only files, write-only files. ( only supports both )
*/

namespace RGF {



	class RGF_API FileIO {
		public :
			void CreateDir(const std::string& filepath);

			void WriteToStorage(const std::string& filepath, const std::string& string);
			void MoveFileLocation(const std::string& sourceFilepath, const std::string& destinationFilepath);
			void RemoveFile(const std::string& filepath);

			const char* ReadFromStorage(const std::string& filepath);


			bool DoesFileExist(const std::string& filepath) const;
			unsigned int GetFileSize(const std::string& filepath) const;

	};
}