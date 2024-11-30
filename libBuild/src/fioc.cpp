#include "fioc.hpp"
#include <filesystem>
#include <fstream>
#include <sys/stat.h>
#include <chrono>

namespace eutil::fioc
{
    EUTIL_API bool FileExists(const char* path)
    {
        return std::filesystem::exists(path);
    }

    EUTIL_API bool CreateFile(const char* path)
    {
        if(FileExists(path))
            return false;
        
        FILE* file = nullptr;
        fopen_s(&file, path, "w");
        if(file == nullptr)
            return false;

        fclose(file);
        return true;
    }

    EUTIL_API bool RemoveFile(const char* path)
    {
        return std::filesystem::remove(path);
    }

    EUTIL_API bool CopyFile(const char* src, const char* dst)
    {
        return std::filesystem::copy_file(src, dst);
    }

    EUTIL_API void MoveFile(const char* src, const char* dst)
    {
        std::filesystem::rename(src, dst);
    }

    EUTIL_API void RenameFile(const char* src, const char* dst)
    {
        MoveFile(src, dst);
    }

    EUTIL_API bool DirectoryExists(const char* path)
    {
        return std::filesystem::is_directory(path);
    }

    EUTIL_API bool CreateDirectory(const char* path)
    {
        std::filesystem::path p(path);
        if(p.has_extension())
            return false;

        return std::filesystem::create_directory(path);
    }

    EUTIL_API bool CreateDirectoryRecursive(const char* path)
    {
        std::filesystem::path p(path);
        //cut filename and extension if exists
        if(p.has_extension())
            p = p.parent_path();

        if(p.empty())
            return false;

        return std::filesystem::create_directories(p);
    }

    EUTIL_API bool RemoveDirectory(const char* path)
    {
        std::filesystem::path p(path);
        if(p.has_extension())
            return false;

        return std::filesystem::remove(path);
    }

    EUTIL_API void CopyDirectory(const char* src, const char* dst)
    {
        std::filesystem::path s(src), d(dst);
        if(s.has_extension() || d.has_extension())
            return;

        std::filesystem::copy(src, dst, std::filesystem::copy_options::recursive);
    }

    EUTIL_API void MoveDirectory(const char* src, const char* dst)
    {
        std::filesystem::path s(src), d(dst);
        if(s.has_extension() || d.has_extension())
            return;

        std::filesystem::rename(src, dst);
    }

    EUTIL_API void RenameDirectory(const char* src, const char* dst)
    {
        MoveDirectory(src, dst);
    }

    EUTIL_API bool GetFileList(const char* path, std::vector<std::string>& files, bool recursive)
    {
        if(!DirectoryExists(path))
            return false;

        for(auto& entry : std::filesystem::directory_iterator(path))
        {
            if(entry.is_directory())
            {
                if(recursive)
                    GetFileList(entry.path().string().c_str(), files, recursive);
            }
            else
            {
                files.push_back(entry.path().string());
            }
        }

        return true;
    }

    EUTIL_API bool GetFileAttributes(const char* path, unsigned int& attributes)
    {
        if(!FileExists(path))
            return false;

        attributes = static_cast<unsigned int>(std::filesystem::status(path).permissions());
        return true;
    }

    EUTIL_API bool SetFileAttributes(const char* path, unsigned int attributes)
    {
        if(!FileExists(path))
            return false;

        std::filesystem::permissions(path, static_cast<std::filesystem::perms>(attributes));
        return true;
    }

    EUTIL_API bool GetFileSize(const char* path, unsigned long long& size)
    {
        if(!FileExists(path))
            return false;

        size = std::filesystem::file_size(path);
        return true;
    }

    EUTIL_API bool GetFileTime(const char* path, unsigned long long& creation, unsigned long long& last_access, unsigned long long& last_write)
    {
        if(!FileExists(path))
            return false;

        struct stat info;
        stat(path, &info);

        creation = info.st_ctime;
        last_access = info.st_atime;
        last_write = info.st_mtime;

        return true;
    }

    EUTIL_API bool SetFileTime(const char* path, unsigned long long creation, unsigned long long last_access, unsigned long long last_write)
    {
        if(!FileExists(path))
            return false;
        //TODO: Implement
        return false;
    }

    EUTIL_API bool IsFileOpen(const FILE* file)
    {
        return file != nullptr;
    }

    EUTIL_API bool open_file(const char* path, const char* mode, FILE** file)
    {
        if(FileExists(path) && IsFileOpen(*file))
            return false;
        
        auto err = fopen_s(file, path, mode);
        return err == 0;
    }

    EUTIL_API void close_file(FILE** file, bool close)
    {
        if(IsFileOpen(*file) && close)
        {
            fclose(*file);
            *file = nullptr;
        }
    }

    EUTIL_API bool ReadDataFromFileRaw(FILE** file, const char* path, void** data, size_t fsize, size_t asize, bool close)
    {
        if(!IsFileOpen(*file))
            open_file(path, "rb", file);

        if(asize < fsize)
            asize = fsize;

        *data = malloc(asize);
        if(data == nullptr)
        {
            close_file(file, close);
            return false;
        }

        auto rsize = fread(*data, 1, fsize, *file);
        close_file(file, close);

        if(rsize != fsize)
        {
            free(*data);
            return false;
        }

        return true;
    }

    EUTIL_API bool ReadDataFromFileRaw(FILE** file, const char* path, void** data, size_t fsize, size_t asize, size_t readOffset, bool close)
    {
        if(!IsFileOpen(*file))
            open_file(path, "rb", file);

        if(asize < fsize)
            asize = fsize;

        *data = malloc(asize);
        if(data == nullptr)
        {
            close_file(file, close);
            return false;
        }

        fseek(*file, readOffset, SEEK_SET);
        auto rsize = fread(*data, 1, fsize, *file);
        close_file(file, close);

        if(rsize != fsize)
        {
            free(*data);
            return false;
        }

        return true;
    }

    EUTIL_API bool WriteDataToFileRaw(FILE** file, const char* path, const void* data, size_t size, bool close)
    {
        if(!IsFileOpen(*file))
            open_file(path, "wb", file);

        auto wsize = fwrite(data, 1, size, *file);
        close_file(file, close);

        return wsize == size;
    }

    EUTIL_API bool AppendDataToFileRaw(FILE** file, const char* path, const void* data, size_t size, bool close)
    {
        if(!IsFileOpen(*file))
            open_file(path, "ab", file);

        auto wsize = fwrite(data, 1, size, *file);
        close_file(file, close);

        return wsize == size;
    }
}