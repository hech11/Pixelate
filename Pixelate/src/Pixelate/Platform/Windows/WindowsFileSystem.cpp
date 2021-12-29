#include "PXpch.h"
#include "Pixelate/Utility/FileSystem.h"


namespace Pixelate {


	static HANDLE s_WatcherThread;
	static std::string s_AssetDirectory = "assets";

	FileWatcherCallback FileSystem::s_Callback;

	bool FileSystem::ShowFileInExplorer(const std::filesystem::path& path) {
		int result = (int)ShellExecute(NULL, L"open", path.wstring().c_str(), NULL, NULL, SW_NORMAL);
		return (result >= 32);

	}
	bool FileSystem::OpenFileExternally(const std::filesystem::path& path) {
		int result = (int)ShellExecute(NULL, 0, path.wstring().c_str(), NULL, NULL, SW_SHOW);
		return (result >= 32);
	}


	void FileSystem::StartWatching() {
		DWORD threadID;
		s_WatcherThread = CreateThread(NULL, 0, Watch, 0, 0, &threadID);
		PX_ASSERT(s_WatcherThread != 0, "Failed to spin thread!");
		
		

	}

	void FileSystem::StopWatching() {
		auto result = WaitForSingleObject(s_WatcherThread, 5000);
		if (result == WAIT_TIMEOUT)
			TerminateThread(s_WatcherThread, 0);
		CloseHandle(s_WatcherThread);
	}

	unsigned long FileSystem::Watch(void* paramenters) {

		std::vector<char> buffer;
		buffer.resize(10 * 1024);

		OVERLAPPED overlapped;
		ZeroMemory(&overlapped, sizeof(OVERLAPPED));
		overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (overlapped.hEvent == NULL)
		{
			PX_CORE_ERROR("Failed to create an overlapped event!\n");
			return 0;
		}


		HANDLE handle = NULL;
		DWORD bytes;


		// retrieve permissions?
		handle = CreateFileA(s_AssetDirectory.c_str(), FILE_LIST_DIRECTORY,
			FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
			NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
			NULL);

		if (handle == INVALID_HANDLE_VALUE) {
			PX_CORE_ERROR("Failed to get the asset directory handle!");
		}


		while (true) {
			ReadDirectoryChangesW(handle, &buffer[0], (uint32_t)buffer.size(), TRUE,
				FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME,
				&bytes,
				&overlapped,
				NULL);

			DWORD waiting = WaitForSingleObject(overlapped.hEvent, 10000);
			if (waiting != WAIT_OBJECT_0)
				continue;

			std::filesystem::path oldPath;
			char filename[1024 * 10];
			char* data = buffer.data();

			while (true) {

				FILE_NOTIFY_INFORMATION& fInfo = *(FILE_NOTIFY_INFORMATION*)data;
				ZeroMemory(filename, sizeof(filename));
				WideCharToMultiByte(0, 0, fInfo.FileName, fInfo.FileNameLength / sizeof(WCHAR), filename, sizeof(filename), 0, 0);
				std::filesystem::path f = std::string(filename);
				std::filesystem::path filepath = s_AssetDirectory / f;
				std::filesystem::path ext = filepath.extension();
				
				std::size_t isTemp = ext.string().rfind(".TMP");
				if (isTemp != std::string::npos)
				{
					if (!fInfo.NextEntryOffset)
						break;
					data += fInfo.NextEntryOffset;
					continue;
				}

				FileWatcherCallbackData callbackData;
				callbackData.Filepath = filepath;
				callbackData.OldFilepath = filepath;
				callbackData.IsDirectory = IsDirectory(filepath);


				// dispatch events
				switch (fInfo.Action)
				{
					case FILE_ACTION_ADDED:
					{
						callbackData.Action = FileSystemAction::Added;
						s_Callback(callbackData);
						break;
					}
					case FILE_ACTION_REMOVED:
					{
						callbackData.Action = FileSystemAction::Deleted;
						s_Callback(callbackData);
						break;
					}
					case FILE_ACTION_MODIFIED:
					{
						callbackData.Action = FileSystemAction::Modified;
						s_Callback(callbackData);
						break;
					}
					case FILE_ACTION_RENAMED_NEW_NAME:
					{
						callbackData.Action = FileSystemAction::Renamed;
						callbackData.OldFilepath = oldPath;

						s_Callback(callbackData);

						break;
					}
					case FILE_ACTION_RENAMED_OLD_NAME:
					{
						oldPath = filepath;
						break;
					}

				}

				if (!fInfo.NextEntryOffset)
					break;
				data += fInfo.NextEntryOffset;
			}
		}

		return 0;
	}
}