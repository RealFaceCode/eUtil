#include "file.hpp"
#include "elog.hpp"

namespace eutil
{
#pragma region Filesystem

    bool FileExists(const std::filesystem::path& path)
    {
        return std::filesystem::exists(path);
    }

    bool IsDirectory(const std::filesystem::path& path)
    {
        return std::filesystem::is_directory(path);
    }

    bool IsFile(const std::filesystem::path& path)
    {
        return std::filesystem::is_regular_file(path);
    }

    bool CreateFile(const std::filesystem::path& path)
    {
        if (FileExists(path))
            return false;

        std::ofstream file(path);
        if (!file.is_open())
        {
            elog::Error<"UTIL">("Failed to create file: {}", path.string());
            return false;
        }

        file.close();
        return true;
    }

    bool CreateDirectory(const std::filesystem::path& path)
    {
        if (FileExists(path))
            return false;

        return std::filesystem::create_directory(path);
    }

    bool RemoveFile(const std::filesystem::path& path)
    {
        if (!IsFile(path))
            return false;

        return std::filesystem::remove(path);
    }

    bool RemoveDirectory(const std::filesystem::path& path)
    {
        if (!IsDirectory(path))
            return false;

        return std::filesystem::remove_all(path);
    }

    bool RenameFile(const std::filesystem::path& old_path, std::string_view new_name)
    {
        if (!IsFile(old_path))
            return false;

        auto new_path = old_path;
        new_path.replace_filename(new_name);

        std::error_code err;
        std::filesystem::rename(old_path, new_path, err);

        return err.value() == 0;
    }

    bool RenameDirectory(const std::filesystem::path& old_path, std::string_view new_name)
    {
        if (!IsDirectory(old_path))
            return false;

        auto new_path = old_path;
        new_path.replace_filename(new_name);

        std::error_code err;
        std::filesystem::rename(old_path, new_path, err);
        return err.value() == 0;
    }

    bool CopyFile(const std::filesystem::path& old_path, const std::filesystem::path& new_path)
    {
        if (!IsFile(old_path))
            return false;

        return std::filesystem::copy_file(old_path, new_path);
    }

    bool CopyDirectory(const std::filesystem::path& old_path, const std::filesystem::path& new_path)
    {
        if (!IsDirectory(old_path))
            return false;

        std::error_code err;
        std::filesystem::copy(old_path, new_path, err);

        return err.value() == 0;
    }

    bool MoveFile(const std::filesystem::path& old_path, const std::filesystem::path& new_path)
    {
        if (!IsFile(old_path))
            return false;

        std::error_code err;
        std::filesystem::rename(old_path, new_path, err);

        return err.value() == 0;
    }

    bool MoveDirectory(const std::filesystem::path& old_path, const std::filesystem::path& new_path)
    {
        if (!IsDirectory(old_path))
            return false;

        std::error_code err;
        std::filesystem::rename(old_path, new_path, err);

        return err.value() == 0;
    }

    bool IsEmpty(const std::filesystem::path& path)
    {
        return std::filesystem::is_empty(path);
    }

    bool IsReadable(const std::filesystem::path& path)
    {
        return (std::filesystem::status(path).permissions() & std::filesystem::perms::owner_read) == std::filesystem::perms::owner_read;
    }

    bool IsWritable(const std::filesystem::path& path)
    {
        return (std::filesystem::status(path).permissions() & std::filesystem::perms::owner_write) == std::filesystem::perms::owner_write;
    }

    bool IsExecutable(const std::filesystem::path& path)
    {
        return (std::filesystem::status(path).permissions() & std::filesystem::perms::owner_exec) == std::filesystem::perms::owner_exec;
    }

    bool IsHidden(const std::filesystem::path& path)
    {
        return path.filename().string().front() == '.';
    }

#pragma endregion

    File::File() = default;

    File::File(const std::filesystem::path& path, FileOpenMode mode)
    : m_path(path), m_mode(mode)
    {
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
            elog::Error<"UTIL">("Invalid file open mode: {}", static_cast<int>(m_mode));
            break;
        }
        }
    }

    File::~File()
    {
        close();
    }

    void File::open()
    {
        if (m_isOpen)
            close();

        if(!FileExists(m_path))
            CreateFile(m_path);

        m_file = ::fopen(m_path.string().c_str(), m_openMode.c_str());
        if (m_file == nullptr)
        {
            elog::Error<"UTIL">("Failed to open file: {}", m_path.string());
            return;
        }

        ::fseek(m_file, 0, SEEK_END);
        m_size = ::ftell(m_file);
        ::fseek(m_file, 0, SEEK_SET);
        m_buffer.resize(m_size);
        m_isOpen = true;
    }

    void File::close()
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

    bool File::isOpen() const
    {
        return m_isOpen;
    }

    BinaryArrayBuffer& File::data()
    {
        return m_buffer;
    }

    const BinaryArrayBuffer& File::data() const
    {
        return m_buffer;
    }

    std::filesystem::path File::path() const
    {
        return m_path;
    }

    size_t File::size() const
    {
        return m_size;
    }

    void File::read()
    {
        if (!m_isOpen)
        {
            elog::Error<"UTIL">("File is not open: {}", m_path.string());
            return;
        }

        std::vector<uint8_t> buffer(m_size);

        auto result = ::fread(buffer.data(), 1, m_size, m_file);
        if (result != m_size)
        {
            elog::Error<"UTIL">("Failed to read from file: {}", m_path.string());
            return;
        }

        m_buffer.push_back(buffer.data(), m_size);
    }

    void File::write()
    {
        if (!m_isOpen)
        {
            elog::Error<"UTIL">("File is not open: {}", m_path.string());
            return;
        }

        auto ptr = m_buffer.data().get();
        auto result = ::fwrite(ptr, 1, m_size, m_file);
        if (result != m_size)
        {
            elog::Error<"UTIL">("Failed to write to file: {}", m_path.string());
            return;
        }
    }

    File::operator bool() const
    {
        return m_isOpen;
    }

    template<typename T>
    void File::push_back(const T& value, bool only_str)
    {
        m_buffer.push_back(value, only_str);
        m_size = m_buffer.size();
    }

    template void File::push_back<uint8_t>(const uint8_t& value, bool only_str);
    template void File::push_back<uint16_t>(const uint16_t& value, bool only_str);
    template void File::push_back<uint32_t>(const uint32_t& value, bool only_str);
    template void File::push_back<uint64_t>(const uint64_t& value, bool only_str);
    template void File::push_back<int8_t>(const int8_t& value, bool only_str);
    template void File::push_back<int16_t>(const int16_t& value, bool only_str);
    template void File::push_back<int32_t>(const int32_t& value, bool only_str);
    template void File::push_back<int64_t>(const int64_t& value, bool only_str);
    template void File::push_back<float>(const float& value, bool only_str);
    template void File::push_back<double>(const double& value, bool only_str);
    template void File::push_back<bool>(const bool& value, bool only_str);
    template void File::push_back<char>(const char& value, bool only_str);
    template void File::push_back<wchar_t>(const wchar_t& value, bool only_str);
    template void File::push_back<char16_t>(const char16_t& value, bool only_str);
    template void File::push_back<char32_t>(const char32_t& value, bool only_str);
    template void File::push_back<std::string>(const std::string& value, bool only_str);
    template void File::push_back<std::string_view>(const std::string_view& value, bool only_str);
    template void File::push_back<const std::string&>(const std::string& value, bool only_str);
    template void File::push_back<std::wstring>(const std::wstring& value, bool only_str);
    template void File::push_back<std::wstring_view>(const std::wstring_view& value, bool only_str);
    template void File::push_back<const std::wstring&>(const std::wstring& value, bool only_str);

    template<typename T>
    T File::getNextElement(bool only_str)
    {
        return m_buffer.getNextElement<T>(only_str);
    }

    template uint8_t File::getNextElement<uint8_t>(bool only_str);
    template uint16_t File::getNextElement<uint16_t>(bool only_str);
    template uint32_t File::getNextElement<uint32_t>(bool only_str);
    template uint64_t File::getNextElement<uint64_t>(bool only_str);
    template int8_t File::getNextElement<int8_t>(bool only_str);
    template int16_t File::getNextElement<int16_t>(bool only_str);
    template int32_t File::getNextElement<int32_t>(bool only_str);
    template int64_t File::getNextElement<int64_t>(bool only_str);
    template float File::getNextElement<float>(bool only_str);
    template double File::getNextElement<double>(bool only_str);
    template bool File::getNextElement<bool>(bool only_str);
    template char File::getNextElement<char>(bool only_str);
    template wchar_t File::getNextElement<wchar_t>(bool only_str);
    template char16_t File::getNextElement<char16_t>(bool only_str);
    template char32_t File::getNextElement<char32_t>(bool only_str);
    template std::string File::getNextElement<std::string>(bool only_str);
    template std::wstring File::getNextElement<std::wstring>(bool only_str);

    std::string File::toString()
    {
        return getNextElement<std::string>(true);
    }
} // namespace eutil