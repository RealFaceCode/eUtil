#include "filecore.hpp"

#include <fstream>
#include <cstdint>
#include <vector>

namespace eutil
{
    EUTIL_API bool FileExists(const std::filesystem::path& path)
    {
        return std::filesystem::exists(path);
    }

    EUTIL_API bool IsDirectory(const std::filesystem::path& path)
    {
        return std::filesystem::is_directory(path);
    }

    EUTIL_API bool IsFile(const std::filesystem::path& path)
    {
        return std::filesystem::is_regular_file(path);
    }

    EUTIL_API bool CreateFile(const std::filesystem::path& path)
    {
        if (FileExists(path))
            return false;

        std::ofstream file(path);
        if (!file.is_open())
        {
            //TODO: log error
            return false;
        }

        file.close();
        return true;
    }

    EUTIL_API bool CreateDirectory(const std::filesystem::path& path)
    {
        if (FileExists(path))
            return false;

        return std::filesystem::create_directory(path);
    }

    EUTIL_API bool RemoveFile(const std::filesystem::path& path)
    {
        if (!IsFile(path))
            return false;

        return std::filesystem::remove(path);
    }

    EUTIL_API bool RemoveDirectory(const std::filesystem::path& path)
    {
        if (!IsDirectory(path))
            return false;

        return std::filesystem::remove_all(path);
    }

    EUTIL_API bool RenameFile(const std::filesystem::path& old_path, std::string_view new_name)
    {
        if (!IsFile(old_path))
            return false;

        auto new_path = old_path;
        new_path.replace_filename(new_name);

        std::error_code err;
        std::filesystem::rename(old_path, new_path, err);

        return err.value() == 0;
    }

    EUTIL_API bool RenameDirectory(const std::filesystem::path& old_path, std::string_view new_name)
    {
        if (!IsDirectory(old_path))
            return false;

        auto new_path = old_path;
        new_path.replace_filename(new_name);

        std::error_code err;
        std::filesystem::rename(old_path, new_path, err);
        return err.value() == 0;
    }

    EUTIL_API bool CopyFile(const std::filesystem::path& old_path, const std::filesystem::path& new_path)
    {
        if (!IsFile(old_path))
            return false;

        return std::filesystem::copy_file(old_path, new_path);
    }

    EUTIL_API bool CopyDirectory(const std::filesystem::path& old_path, const std::filesystem::path& new_path)
    {
        if (!IsDirectory(old_path))
            return false;

        std::error_code err;
        std::filesystem::copy(old_path, new_path, err);

        return err.value() == 0;
    }

    EUTIL_API bool MoveFile(const std::filesystem::path& old_path, const std::filesystem::path& new_path)
    {
        if (!IsFile(old_path))
            return false;

        std::error_code err;
        std::filesystem::rename(old_path, new_path, err);

        return err.value() == 0;
    }

    EUTIL_API bool MoveDirectory(const std::filesystem::path& old_path, const std::filesystem::path& new_path)
    {
        if (!IsDirectory(old_path))
            return false;

        std::error_code err;
        std::filesystem::rename(old_path, new_path, err);

        return err.value() == 0;
    }

    EUTIL_API bool IsEmpty(const std::filesystem::path& path)
    {
        return std::filesystem::is_empty(path);
    }

    EUTIL_API bool IsReadable(const std::filesystem::path& path)
    {
        return (std::filesystem::status(path).permissions() & std::filesystem::perms::owner_read) == std::filesystem::perms::owner_read;
    }

    EUTIL_API bool IsWritable(const std::filesystem::path& path)
    {
        return (std::filesystem::status(path).permissions() & std::filesystem::perms::owner_write) == std::filesystem::perms::owner_write;
    }

    EUTIL_API bool IsExecutable(const std::filesystem::path& path)
    {
        return (std::filesystem::status(path).permissions() & std::filesystem::perms::owner_exec) == std::filesystem::perms::owner_exec;
    }

    EUTIL_API bool IsHidden(const std::filesystem::path& path)
    {
        return path.filename().string().front() == '.';
    }

    EUTIL_API bool IsSymlink(const std::filesystem::path& path)
    {
        return std::filesystem::is_symlink(path);
    }

    EUTIL_API bool IsRelative(const std::filesystem::path& path)
    {
        return path.is_relative();
    }

    EUTIL_API bool IsAbsolute(const std::filesystem::path& path)
    {
        return path.is_absolute();
    }

    EUTIL_API size_t FileSize(const std::filesystem::path& path)
    {
        return std::filesystem::file_size(path);
    }

    EUTIL_API std::string FileExtension(const std::filesystem::path& path)
    {
        return path.extension().string();
    }

    EUTIL_API std::string FileName(const std::filesystem::path& path)
    {
        return path.filename().string();
    }

    EUTIL_API std::string FileNameWithoutExtension(const std::filesystem::path& path)
    {
        return path.stem().string();
    }

    EUTIL_API std::string FileDirectory(const std::filesystem::path& path)
    {
        return path.parent_path().string();
    }

    EUTIL_API std::string FileDirectoryRoot(const std::filesystem::path& path)
    {
        return path.root_directory().string();
    }

    EUTIL_API std::string FileCanonical(const std::filesystem::path& path)
    {
        return std::filesystem::canonical(path).string();
    }

    EUTIL_API std::string FileRelative(const std::filesystem::path& path, const std::filesystem::path& base)
    {
        return std::filesystem::relative(path, base).string();
    }

    EUTIL_API std::string FileAbsolute(const std::filesystem::path& path)
    {
        return std::filesystem::absolute(path).string();
    }

    EUTIL_API std::string FileNormalize(const std::filesystem::path& path)
    {
        return std::filesystem::weakly_canonical(path).string();
    }

    EUTIL_API std::shared_ptr<uint8_t[]> ReadFile(const std::filesystem::path& path, bool putZeroAtEnd)
    {
        if (!IsFile(path))
            return nullptr;

        auto size = FileSize(path);
        auto buffer = std::make_shared<uint8_t[]>(putZeroAtEnd ? size + 1 : size);

        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
            return nullptr;

        file.read(reinterpret_cast<char*>(buffer.get()), size);
        file.close();

        if (putZeroAtEnd)
            buffer[size] = '\0';

        return buffer;
    }
    
    EUTIL_API std::shared_ptr<uint8_t[]> ReadFile(std::ifstream& file, size_t size)
    {
        if(!file.is_open())
            return nullptr;

        auto buffer = std::make_shared<uint8_t[]>(size);
        file.read(reinterpret_cast<char*>(buffer.get()), size);
        return buffer;
    }

    EUTIL_API BinaryBuffer ReadFileBuffer(const std::filesystem::path& path)
    {
        if (!IsFile(path))
            return BinaryBuffer();

        auto size = FileSize(path);
        BinaryBuffer buffer;
        buffer.reserve(size);

        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
            return buffer;

        auto data = ReadFile(file, size);

        buffer.push(data.get());
        file.close();

        return buffer;
    }

    EUTIL_API BinaryBuffer ReadFileBuffer(std::ifstream& file, size_t size)
    {
        if(!file.is_open())
            return BinaryBuffer();

        BinaryBuffer buffer;
        buffer.reserve(size);

        auto data = ReadFile(file, size);
        buffer.push(data.get());

        return buffer;
    }

    EUTIL_API bool WriteFile(const std::filesystem::path& path, std::shared_ptr<uint8_t[]> data, size_t size)
    {
        if (IsFile(path))
            return false;

        std::ofstream file(path, std::ios::binary);
        if (!file.is_open())
            return false;

        file.write(reinterpret_cast<char*>(data.get()), size);
        file.close();

        return true;
    }
    
    EUTIL_API bool WriteFile(const std::filesystem::path& path, std::string_view data)
    {
        if (IsFile(path))
            return false;

        std::ofstream file(path);
        if (!file.is_open())
            return false;

        file << data;
        file.close();

        return true;
    }

    EUTIL_API bool WriteFile(std::ofstream& file, std::shared_ptr<uint8_t[]> data, size_t size)
    {
        if(!file.is_open())
            return false;

        file.write(reinterpret_cast<char*>(data.get()), size);
        return true;
    }

    EUTIL_API bool WriteFile(std::ofstream& file, std::string_view data)
    {
        if(!file.is_open())
            return false;

        file << data;
        return true;
    }

    EUTIL_API bool WriteFile(const std::filesystem::path& path, const BinaryBuffer& buffer)
    {
        if (IsFile(path))
            return false;

        std::ofstream file(path, std::ios::binary);
        if (!file.is_open())
            return false;

        file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        file.close();

        return true;
    }

    EUTIL_API bool AppendFile(const std::filesystem::path& path, std::shared_ptr<uint8_t[]> data, size_t size)
    {
        if (!IsFile(path))
            return false;

        std::ofstream file(path, std::ios::binary | std::ios::app);
        if (!file.is_open())
            return false;

        file.write(reinterpret_cast<char*>(data.get()), size);
        file.close();

        return true;
    }

    EUTIL_API bool AppendFile(const std::filesystem::path& path, std::string_view data)
    {
        if (!IsFile(path))
            return false;

        std::ofstream file(path, std::ios::app);
        if (!file.is_open())
            return false;

        file << data;
        file.close();

        return true;
    }

    EUTIL_API bool AppendFile(std::ofstream& file, std::shared_ptr<uint8_t[]> data, size_t size)
    {
        if(!file.is_open())
            return false;

        file.write(reinterpret_cast<char*>(data.get()), size);
        return true;
    }

    EUTIL_API bool AppendFile(std::ofstream& file, std::string_view data)
    {
        if(!file.is_open())
            return false;
            
        file << data;
        return true;
    }

    EUTIL_API bool AppendFile(const std::filesystem::path& path, const BinaryBuffer& buffer)
    {
        if (!IsFile(path))
            return false;

        std::ofstream file(path, std::ios::binary | std::ios::app);
        if (!file.is_open())
            return false;

        file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
        file.close();

        return true;
    }
} // namespace eutil