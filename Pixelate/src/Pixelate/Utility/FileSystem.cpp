#include "PXpch.h"
#include "FileSystem.h"


#include <fstream>


namespace Pixelate {



	void FileSystem::CreateDirectory(const std::string& directory)
	{
		std::filesystem::create_directories(directory);
	}

	void FileSystem::CreateDirectory(const std::filesystem::path& directory)
	{
		std::filesystem::create_directories(directory);
	}

	void FileSystem::DeleteDirectory(const std::string& directory)
	{
		std::filesystem::remove_all(directory);
	}

	void FileSystem::DeleteDirectory(const std::filesystem::path& directory)
	{
		std::filesystem::remove_all(directory);
	}

	void FileSystem::RenameDirectory(const std::filesystem::path& oldDirectory, const std::string& newName)
	{
		std::filesystem::path newDir = oldDirectory;
		newDir = newDir.parent_path();
		newDir /= newName;
		std::filesystem::rename(oldDirectory, newDir);
	}

	void FileSystem::RenameDirectory(const std::string& oldDirectory, const std::string& newName)
	{
		std::filesystem::path newDir = oldDirectory;
		newDir = newDir.parent_path();
		newDir /= newName;
		std::filesystem::rename(oldDirectory, newDir);
	}

	void FileSystem::MoveDirectory(const std::string& oldDirectory, const std::string& newDirectory)
	{
		auto copyOptions = std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive;
		std::filesystem::copy(oldDirectory, newDirectory, copyOptions);
		DeleteDirectory(oldDirectory);
	}

	void FileSystem::MoveDirectory(const std::filesystem::path& oldDirectory, const std::filesystem::path& newDirectory)
	{
		auto copyOptions = std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive;
		std::filesystem::copy(oldDirectory, newDirectory, copyOptions);
		DeleteDirectory(oldDirectory);
	}

	
	
	void FileSystem::CopyDirectory(const std::string& oldDirectory, const std::string& newDirectory)
	{
		std::filesystem::copy_options options = std::filesystem::copy_options::recursive | std::filesystem::copy_options::update_existing;
		std::filesystem::copy(oldDirectory, newDirectory, options);
	}

	void FileSystem::CopyDirectory(const std::filesystem::path& oldDirectory, const std::filesystem::path& newDirectory)
	{
		std::filesystem::copy_options options = std::filesystem::copy_options::recursive | std::filesystem::copy_options::update_existing;
		std::filesystem::copy(oldDirectory, newDirectory, options);
	}

	void FileSystem::WriteText(const std::filesystem::path& filepath, const std::string& string)
	{
		std::fstream file(filepath);
		file.write(string.c_str(), string.size());
		file.close();
	}

	void FileSystem::WriteText(const std::string& filepath, const std::string& string)
	{
		std::fstream file(filepath);
		file.write(string.c_str(), string.size());
		file.close();
	}

	void FileSystem::MoveFile(const std::string& srcFilePath, const std::string& destFilePath) {
		auto copyOptions = std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive;
		std::filesystem::copy(srcFilePath, destFilePath, copyOptions);
		DeleteFile(srcFilePath);
	}

	void FileSystem::MoveFile(const std::filesystem::path& srcFilePath, const std::filesystem::path& destFilePath)
	{
		auto copyOptions = std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive;
		std::filesystem::copy(srcFilePath, destFilePath, copyOptions);
		DeleteFile(srcFilePath);
	}

	void FileSystem::DeleteFile(const std::string& filepath)
	{
		std::filesystem::remove(filepath);

	}

	void FileSystem::DeleteFile(const std::filesystem::path& filepath)
	{
		std::filesystem::remove(filepath);
	}

	void FileSystem::ReadText(const std::string& filepath, std::string& outBuffer)
	{
		std::ifstream file(filepath);
		outBuffer.clear();

		file.seekg(0, std::ios::end);
		outBuffer.reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		outBuffer.assign((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	}

	void FileSystem::ReadText(const std::filesystem::path& filepath, std::string& outBuffer)
	{
		std::ifstream file(filepath);
		outBuffer.clear();

		file.seekg(0, std::ios::end);
		outBuffer.reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		outBuffer.assign((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	}

	bool FileSystem::Exists(const std::string& filepath)
	{
		return std::filesystem::exists(filepath);
	}

	bool FileSystem::Exists(const std::filesystem::path& path)
	{
		return std::filesystem::exists(path);
	}

	unsigned int FileSystem::GetSize(const std::string& filepath)
	{
		return std::filesystem::file_size(filepath);
	}

	unsigned int FileSystem::GetSize(const std::filesystem::path& filepath)
	{
		return std::filesystem::file_size(filepath);

	}


	bool FileSystem::IsDirectory(const std::string& filepath)
	{
		return std::filesystem::is_directory(filepath);
	}

	bool FileSystem::IsDirectory(const std::filesystem::path& filepath)
	{
		return std::filesystem::is_directory(filepath);
	}

} 