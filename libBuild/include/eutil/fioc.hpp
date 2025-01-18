#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include "defines.hpp"

namespace eutil::fioc 
{
    EUTIL_API bool FileExists(const char* path);
    EUTIL_API bool FileExists(const std::filesystem::path& path);
    EUTIL_API bool CreateFile(const char* path);
    EUTIL_API bool CreateFile(const std::filesystem::path& path);
    EUTIL_API bool RemoveFile(const char* path);
    EUTIL_API bool RemoveFile(const std::filesystem::path& path);
    EUTIL_API bool CopyFile(const char* src, const char* dst);
    EUTIL_API bool CopyFile(const std::filesystem::path& src, const std::filesystem::path& dst);
    EUTIL_API void MoveFile(const char* src, const char* dst);
    EUTIL_API void MoveFile(const std::filesystem::path& src, const std::filesystem::path& dst);
    EUTIL_API void RenameFile(const char* src, const char* dst);
    EUTIL_API void RenameFile(const std::filesystem::path& src, const std::filesystem::path& dst);
    EUTIL_API bool DirectoryExists(const char* path);
    EUTIL_API bool DirectoryExists(const std::filesystem::path& path);
    EUTIL_API bool CreateDirectory(const char* path);
    EUTIL_API bool CreateDirectory(const std::filesystem::path& path);
    EUTIL_API bool CreateDirectoryRecursive(const char* path);
    EUTIL_API bool CreateDirectoryRecursive(const std::filesystem::path& path);
    EUTIL_API bool RemoveDirectory(const char* path);
    EUTIL_API bool RemoveDirectory(const std::filesystem::path& path);
    EUTIL_API void CopyDirectory(const char* src, const char* dst);
    EUTIL_API void CopyDirectory(const std::filesystem::path& src, const std::filesystem::path& dst);
    EUTIL_API void MoveDirectory(const char* src, const char* dst);
    EUTIL_API void MoveDirectory(const std::filesystem::path& src, const std::filesystem::path& dst);
    EUTIL_API void RenameDirectory(const char* src, const char* dst);
    EUTIL_API void RenameDirectory(const std::filesystem::path& src, const std::filesystem::path& dst);
    EUTIL_API bool GetFileList(const char* path, std::vector<std::string>& files, bool recursive = false);
    EUTIL_API bool GetFileAttributes(const char* path, unsigned int& attributes);
    EUTIL_API bool SetFileAttributes(const char* path, unsigned int attributes);
    EUTIL_API bool GetFileSize(const char* path, unsigned long long& size);
    EUTIL_API bool GetFileTime(const char* path, unsigned long long& creation, unsigned long long& last_access, unsigned long long& last_write);
    EUTIL_API bool SetFileTime(const char* path, unsigned long long creation, unsigned long long last_access, unsigned long long last_write);

    EUTIL_API bool IsFileOpen(const FILE* file);
    EUTIL_API bool open_file(const char* path, const char* mode, FILE** file);
    EUTIL_API void close_file(FILE** file, bool close);
    EUTIL_API bool ReadDataFromFileRaw(FILE** file, const char* path, void** data, size_t fsize, size_t asize, bool close = true);
    EUTIL_API bool ReadDataFromFileRaw(FILE** file, const char* path, void** data, size_t fsize, size_t asize, size_t readOffset, bool close = true);
    EUTIL_API bool WriteDataToFileRaw(FILE** file, const char* path, const void* data, size_t size, bool close = true);
    EUTIL_API bool AppendDataToFileRaw(FILE** file, const char* path, const void* data, size_t size, bool close = true);
}