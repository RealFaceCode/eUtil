#include "eutil/filesystem/FileIOGeneric.hpp"

#include "eutil/buffer/FileAccessTimes.hpp"
#include "eutil/buffer/FilePermission.hpp"

static util::FileAccessTimes GetFileTimes(const std::filesystem::path& path);
static bool SetFileTimes(const std::filesystem::path& path, const util::FileAccessTimes& times);

#pragma region System Specific Functions

#ifdef _WIN32
#include <Windows.h>

static util::FileAccessTimes GetFileTimes(const std::filesystem::path& path)
{
    util::FileAccessTimes times;

    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExA(path.string().c_str(), GetFileExInfoStandard, &data))
        return times;

    FILETIME creation;
    FILETIME lastAccess;
    FILETIME lastWrite;
    creation = data.ftCreationTime;
    lastAccess = data.ftLastAccessTime;
    lastWrite = data.ftLastWriteTime;

    times.creation = (static_cast<unsigned long long>(creation.dwHighDateTime) << 32) | creation.dwLowDateTime;
    times.lastAccess = (static_cast<unsigned long long>(lastAccess.dwHighDateTime) << 32) | lastAccess.dwLowDateTime;
    times.lastWrite = (static_cast<unsigned long long>(lastWrite.dwHighDateTime) << 32) | lastWrite.dwLowDateTime;

    return times;
}

static bool SetFileTimes(const std::filesystem::path& path, const util::FileAccessTimes& times)
{
    FILETIME creation;
    FILETIME lastAccess;
    FILETIME lastWrite;
    creation.dwHighDateTime = static_cast<DWORD>(times.creation >> 32);
    creation.dwLowDateTime = static_cast<DWORD>(times.creation);
    lastAccess.dwHighDateTime = static_cast<DWORD>(times.lastAccess >> 32);
    lastAccess.dwLowDateTime = static_cast<DWORD>(times.lastAccess);
    lastWrite.dwHighDateTime = static_cast<DWORD>(times.lastWrite >> 32);
    lastWrite.dwLowDateTime = static_cast<DWORD>(times.lastWrite);

    HANDLE file = CreateFileA(path.string().c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE)
        return false;

    if (!::SetFileTime(file, &creation, &lastAccess, &lastWrite))
    {
        ::CloseHandle(file);
        return false;
    }

    ::CloseHandle(file);
    return true;
}

#elif __linux__
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

static util::FileAccessTimes GetFileTimes(const std::filesystem::path& path)
{
    util::FileAccessTimes times;

    struct stat data;
    if (stat(path.string().c_str(), &data) != 0)
        return times;

    times.creation = data.st_ctime;
    times.lastAccess = data.st_atime;
    times.lastWrite = data.st_mtime;

    return times;
}

static bool SetFileTimes(const std::filesystem::path& path, const util::FileAccessTimes& times)
{
    struct utimbuf utime;
    utime.actime = times.lastAccess;
    utime.modtime = times.lastWrite;

    if (utime(path.string().c_str(), &utime) != 0)
        return false;

    return true;
}

#elif __APPLE__
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

static util::FileAccessTimes GetFileTimes(const std::filesystem::path& path)
{
    util::FileAccessTimes times;

    struct stat data;
    if (stat(path.string().c_str(), &data) != 0)
        return times;

    times.creation = data.st_ctime;
    times.lastAccess = data.st_atime;
    times.lastWrite = data.st_mtime;

    return times;
}

static bool SetFileTimes(const std::filesystem::path& path, const util::FileAccessTimes& times)
{
    struct utimbuf utime;
    utime.actime = times.lastAccess;
    utime.modtime = times.lastWrite;

    if (utime(path.string().c_str(), &utime) != 0)
        return false;

    return true;
}

#else
#error "Platform not supported"
#endif

#pragma endregion System Specific Functions

namespace util
{
    bool FileExist(const std::filesystem::path& path)
    {
        return std::filesystem::exists(path);
    }

    bool CreateFile(const std::filesystem::path& path)
    {
        if (FileExist(path))
            return false;

        FILE* file;
        ::fopen_s(&file, path.string().c_str(), "w");
        if(file == nullptr)
            return false;

        ::fclose(file);

        return true;
    }

    bool RemoveFile(const std::filesystem::path& path)
    {
        if (!FileExist(path))
            return false;

        return std::filesystem::remove(path);
    }

    bool CopyFile(const std::filesystem::path& src, const std::filesystem::path& dst)
    {
        if (!FileExist(src))
            return false;

        return std::filesystem::copy_file(src, dst);
    }

    void MoveFile(const std::filesystem::path& src, const std::filesystem::path& dst)
    {
        if (!FileExist(src))
            return;

        std::filesystem::rename(src, dst);
    }

    bool RenameFile(const std::filesystem::path& src, const std::filesystem::path& dst)
    {
        auto strippedSrc = src.parent_path();
        auto strippedDst = dst.parent_path();

        if (strippedSrc != strippedDst)
            return false;

        MoveFile(src, dst);
        return true;
    }

    bool DirectoryExists(const std::filesystem::path& path)
    {
        if(path.has_extension())
            return false;

        return std::filesystem::is_directory(path);
    }

    bool CreateDirectory(const std::filesystem::path& path)
    {
        if (DirectoryExists(path))
            return false;

        return std::filesystem::create_directory(path);
    }

    bool CreateDirectorys(const std::filesystem::path& path)
    {
        if (DirectoryExists(path))
            return false;

        return std::filesystem::create_directories(path);
    }

    bool RemoveDirectory(const std::filesystem::path& path)
    {
        if (!DirectoryExists(path))
            return false;

        return std::filesystem::remove(path);
    }

    void CopyDirectory(const std::filesystem::path& src, const std::filesystem::path& dst)
    {
        if (!DirectoryExists(src))
            return;

        std::filesystem::copy(src, dst, std::filesystem::copy_options::recursive);
    }

    void MoveDirectory(const std::filesystem::path& src, const std::filesystem::path& dst)
    {
        if (!DirectoryExists(src))
            return;

        std::filesystem::rename(src, dst);
    }
    
    bool RenameDirectory(const std::filesystem::path& src, const std::filesystem::path& dst)
    {
        auto strippedSrc = src.parent_path();
        auto strippedDst = dst.parent_path();

        if (strippedSrc != strippedDst)
            return false;

        MoveDirectory(src, dst);
        return true;
    }

    std::vector<std::filesystem::path> GetFileList(const std::filesystem::path& path, bool recursive)
    {
        std::vector<std::filesystem::path> files;

        if (recursive)
        {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
            {
                if (entry.is_regular_file())
                    files.push_back(entry.path());
            }
        }
        else
        {
            for (const auto& entry : std::filesystem::directory_iterator(path))
            {
                if (entry.is_regular_file())
                    files.push_back(entry.path());
            }
        }

        return files;
    }

    size_t GetFileSize(const std::filesystem::path& path)
    {
        if (!FileExist(path))
            return -1;
        return std::filesystem::file_size(path);
    }

    std::optional<FileAccessTimes> GetFileTime(const std::filesystem::path& path)
    {
        if (!FileExist(path))
            return std::nullopt;

        return GetFileTimes(path);
    }

    bool SetFileTime(const std::filesystem::path& path, const FileAccessTimes& times)
    {
        if (!FileExist(path))
            return false;

        
        return SetFileTimes(path, times);
    }

    std::optional<FilePermission> GetFilePermission(const std::filesystem::path& path)
    {
        if (!FileExist(path))
            return std::nullopt;

        FilePermission permission;

        std::filesystem::perms filePermissions = std::filesystem::status(path).permissions();

        permission.read = (filePermissions & std::filesystem::perms::owner_read) != std::filesystem::perms::none;
        permission.write = (filePermissions & std::filesystem::perms::owner_write) != std::filesystem::perms::none;
        permission.execute = (filePermissions & std::filesystem::perms::owner_exec) != std::filesystem::perms::none;

        return permission;
    }

    bool SetFilePermission(const std::filesystem::path& path, const FilePermission& permission)
    {
        if (!FileExist(path))
            return false;

        std::filesystem::perms filePermissions = std::filesystem::perms::none;

        if (permission.read)
            filePermissions |= std::filesystem::perms::owner_read;
        if (permission.write)
            filePermissions |= std::filesystem::perms::owner_write;
        if (permission.execute)
            filePermissions |= std::filesystem::perms::owner_exec;

        std::filesystem::permissions(path, filePermissions);

        return true;
    }
}