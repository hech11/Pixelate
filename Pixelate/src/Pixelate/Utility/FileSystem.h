#pragma once

#include <string>

#include <filesystem>

namespace Pixelate {


	enum class FileSystemAction { None = -1, Added, Deleted, Modified, Renamed };

	struct FileWatcherCallbackData {
		FileSystemAction Action;
		std::filesystem::path Filepath;
		std::filesystem::path OldFilepath;
		bool IsDirectory;
	};

	using FileWatcherCallback = std::function<void(FileWatcherCallbackData)>;

	//TODO: Support buffer streams and not just c++ strings
	class FileSystem {

		public :
			static void CreateDirectory(const std::string& directory);
			static void CreateDirectory(const std::filesystem::path& directory);

			static void DeleteDirectory(const std::string& directory);
			static void DeleteDirectory(const std::filesystem::path& directory);

			static void RenameDirectory(const std::string& oldDirectory, const std::string& newName);
			static void RenameDirectory(const std::filesystem::path& oldDirectory, const std::string& newName);

			static void MoveDirectory(const std::filesystem::path& oldDirectory, const std::filesystem::path& newDirectory);

			static void CopyDirectory(const std::string& oldDirectory, const std::string& newDirectory);
			static void CopyDirectory(const std::filesystem::path& oldDirectory, const std::filesystem::path& newDirectory);

			static void WriteText(const std::string& filepath, const std::string& string);
			static void WriteText(const std::filesystem::path& filepath, const std::string& string);


			static void MoveFile(const std::string& srcFilePath, const std::string& destFilePath);
			static void MoveFile(const std::filesystem::path& srcFilePath, const std::filesystem::path& destFilePath);

			static void DeleteFile(const std::string& filepath);
			static void DeleteFile(const std::filesystem::path& filepath);

			static std::string ReadText(const std::string& filepath);
			static std::string ReadText(const std::filesystem::path& filepath);


			static bool Exists(const std::string& filepath);
			static bool Exists(const std::filesystem::path& path);


			static unsigned int GetSize(const std::string& filepath);
			static unsigned int GetSize(const std::filesystem::path& filepath);

			static bool IsDirectory(const std::string& filepath);
			static bool IsDirectory(const std::filesystem::path& filepath);
			

			static void SetFileWatcherCallback(const FileWatcherCallback& callback) { s_Callback = callback; }

			// To be implemented in "Platform/[OS]/[OS]FileSystem.cpp... for example, the windows code is at Platform/Windows/WindowsFile.cpp"
			static bool ShowFileInExplorer(const std::filesystem::path& path);
			static bool OpenFileExternally(const std::filesystem::path& path);

			static void StartWatching();
			static void StopWatching();

		private :
			static unsigned long Watch(void* paramenters);
		private :
			static FileWatcherCallback s_Callback;
	};

}