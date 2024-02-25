#include "file.hpp"
#include "elog.hpp"

namespace eutil
{
    File::File() = default;

    File::File(const std::filesystem::path& path)
    : m_path(path)
    {}

    File::~File()
    {
        close();
    }

    void File::open()
    {
        if (m_isOpen)
            close();

        m_file = ::fopen(m_path.string().c_str(), "rb");
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

        ::fread(buffer.data(), 1, m_size, m_file);

        m_buffer.push_back(buffer.data(), m_size);
    }

    void File::write()
    {
        if (!m_isOpen)
        {
            elog::Error<"UTIL">("File is not open: {}", m_path.string());
            return;
        }

        ::fwrite(m_buffer.data().get(), 1, m_size, m_file);
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