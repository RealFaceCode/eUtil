#pragma once

#include "eutil/defines.hpp"

#include <filesystem>
#include <vector>
#include <optional>	

namespace util
{
    struct FileAccessTimes;
    struct FilePermission;

#pragma region Generic Functions

    EUTIL_API bool FileExist(const std::filesystem::path& path);
    EUTIL_API bool CreateFile(const std::filesystem::path& path);
    EUTIL_API bool RemoveFile(const std::filesystem::path& path);
    EUTIL_API bool CopyFile(const std::filesystem::path& src, const std::filesystem::path& dst);
    EUTIL_API void MoveFile(const std::filesystem::path& src, const std::filesystem::path& dst);
    EUTIL_API bool RenameFile(const std::filesystem::path& src, const std::filesystem::path& dst);

    EUTIL_API bool DirectoryExists(const std::filesystem::path& path);
    EUTIL_API bool CreateDirectory(const std::filesystem::path& path);
    EUTIL_API bool CreateDirectorys(const std::filesystem::path& path);
    EUTIL_API bool RemoveDirectory(const std::filesystem::path& path);
    EUTIL_API void CopyDirectory(const std::filesystem::path& src, const std::filesystem::path& dst);
    EUTIL_API void MoveDirectory(const std::filesystem::path& src, const std::filesystem::path& dst);
    EUTIL_API bool RenameDirectory(const std::filesystem::path& src, const std::filesystem::path& dst);

    EUTIL_API std::vector<std::filesystem::path> GetFileList(const std::filesystem::path& path, bool recursive = false);

    EUTIL_API size_t GetFileSize(const std::filesystem::path& path);
    EUTIL_API std::optional<FileAccessTimes> GetFileTime(const std::filesystem::path& path);
    EUTIL_API bool SetFileTime(const std::filesystem::path& path, const FileAccessTimes& times);
    EUTIL_API std::optional<FilePermission> GetFilePermission(const std::filesystem::path& path);
    EUTIL_API bool SetFilePermission(const std::filesystem::path& path, const FilePermission& permission);
#pragma endregion Generic Functions

}