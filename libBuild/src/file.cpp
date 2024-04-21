#include "file.hpp"

#include <fstream>
#include <cstdint>
#include <vector>

namespace eutil
{
#pragma region Filesystem

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

#pragma endregion

    EUTIL_API File::File() = default;

    EUTIL_API File::File(const std::filesystem::path& path, FileOpenMode mode)
    : m_path(path)
    {
        setMode(mode);
    }

    EUTIL_API File::~File()
    {
        close();
    }

    EUTIL_API void File::setPath(const std::filesystem::path& path)
    {
        m_path = path;
    }

    EUTIL_API void File::setMode(FileOpenMode mode)
    {
        m_mode = mode;
        switch (m_mode)
        {
        case FileOpenMode::Read:
        {
            m_openMode = "rb";
            break;
        }
        case FileOpenMode::Write:
        {
            m_openMode = "wb";
            break;
        }
        case FileOpenMode::Append:
        {
            m_openMode = "ab";
            break;
        }
        case FileOpenMode::ReadWrite:
        {
            m_openMode = "r+b";
            break;
        }
        default:
        {
            //TODO: log error
            break;
        }
        }
    }


    EUTIL_API void File::open()
    {
        if (m_isOpen)
            close();

        if(!FileExists(m_path))
            CreateFile(m_path);

        auto err = ::fopen_s(&m_file, m_path.string().c_str(), m_openMode.c_str());
        if (m_file == nullptr)
        {
            //TODO: log error
            return;
        }

        ::fseek(m_file, 0, SEEK_END);
        m_size = ::ftell(m_file);
        ::fseek(m_file, 0, SEEK_SET);
        m_buffer.resize(m_size);
        m_isOpen = true;
    }

    EUTIL_API void File::close()
    {
        if (m_isOpen)
        {
            ::fclose(m_file);
            m_file = nullptr;
            m_isOpen = false;
            m_size = 0;
            m_buffer.clear();
        }
    }

    EUTIL_API bool File::isOpen() const
    {
        return m_isOpen;
    }

    EUTIL_API BinaryArrayBuffer& File::data()
    {
        return m_buffer;
    }

    EUTIL_API const BinaryArrayBuffer& File::data() const
    {
        return m_buffer;
    }

    EUTIL_API std::filesystem::path File::path() const
    {
        return m_path;
    }

    EUTIL_API size_t File::size() const
    {
        return m_size;
    }

    EUTIL_API void File::read()
    {
        if (!m_isOpen)
        {
            //TODO: log error
            return;
        }

        std::vector<uint8_t> buffer(m_size);

        auto result = ::fread(buffer.data(), 1, m_size, m_file);
        if (result != m_size)
        {
            //TODO: log error
            return;
        }

        m_buffer.push_rawData(buffer.data(), m_size);
    }

    EUTIL_API void File::write()
    {
        if (!m_isOpen)
        {
            //TODO: log error
            return;
        }

        auto ptr = m_buffer.data().get();
        auto result = ::fwrite(ptr, 1, m_size, m_file);
        if (result != m_size)
        {
            //TODO: log error
            return;
        }
    }

    EUTIL_API File::operator bool() const
    {
        return m_isOpen;
    }

    EUTIL_API std::string File::toString()
    {
        return getNextElement<std::string>(true);
    }
} // namespace eutil