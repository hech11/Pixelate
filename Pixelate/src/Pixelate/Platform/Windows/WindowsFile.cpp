#include "PXpch.h"
#include "Pixelate/Utility/FileSystem.h"


namespace Pixelate {

	bool FileSystem::ShowFileInExplorer(const std::filesystem::path& path) {
		int result = (int)ShellExecute(NULL, L"open", path.wstring().c_str(), NULL, NULL, SW_NORMAL);
		return (result >= 32);

	}
	bool FileSystem::OpenFileExternally(const std::filesystem::path& path) {
		int result = (int)ShellExecute(NULL, 0, path.wstring().c_str(), NULL, NULL, SW_SHOW);
		return (result >= 32);
	}

}