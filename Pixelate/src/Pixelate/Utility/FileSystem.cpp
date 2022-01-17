#include "PXpch.h"
#include "FileSystem.h"


#include <fstream>


namespace Pixelate {



	void FileSystem::CreateDir(const std::filesystem::path& directory)
	{
		std::filesystem::create_directories(directory);
	}



	void FileSystem::DeleteDirectory(const std::filesystem::path& directory)
	{
		std::filesystem::remove_all(directory);
	}

	void FileSystem::RenameDirectory(const std::filesystem::path& oldDirectory, const std::string& newName)
	{
		std::filesystem::rename(oldDirectory, newName);
	}

	void FileSystem::RenameDirectory(const std::string& oldDirectory, const std::string& newName)
	{
		std::filesystem::rename(oldDirectory, newName);
	}


	void FileSystem::MoveDirectory(const std::filesystem::path& oldDirectory, const std::filesystem::path& newDirectory)
	{
		auto copyOptions = std::filesystem::copy_options::update_existing | std::filesystem::copy_options::recursive;
		std::filesystem::path path = newDirectory / oldDirectory.filename();

		CreateDir(path);

		std::filesystem::copy(oldDirectory, path, copyOptions);
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

	std::string FileSystem::ReadText(const std::string& filepath)
	{
		std::ifstream file(filepath);
		std::string result;

		file.seekg(0, std::ios::end);
		result.reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		result.assign((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

		return result;
	}

	std::string FileSystem::ReadText(const std::filesystem::path& filepath)
	{

		std::ifstream file(filepath);
		std::string result;

		file.seekg(0, std::ios::end);
		result.reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		result.assign((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

		return result;
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