#include "file.hpp"

namespace eutil
{
    File::File() = default;

    File::File(const std::filesystem::path& path)
    : m_path(path)
    {}

    File::File(const File& other) = default;
    File::File(File&& other) noexcept = default;

    File::~File()
    {
        close();
    }

    File& File::operator=(const File& other) = default;

    File& File::operator=(File&& other) noexcept
    {
        m_path = std::move(other.m_path);
        m_buffer = std::move(other.m_buffer);
        m_isOpen = other.m_isOpen;
        m_size = other.m_size;
        m_file = other.m_file;

        other.m_isOpen = false;
        other.m_size = 0;
        other.m_file = nullptr;

        return *this;
    }

    void File::open()
    {
        if (m_isOpen)
            close();

        m_file = ::fopen(m_path.string().c_str(), "rb");
        if (m_file == nullptr)
            throw std::runtime_error("Failed to open file: " + m_path.string());

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
            throw std::runtime_error("File is not open");

        ::fread(m_buffer.data().get(), 1, m_size, m_file);
    }

    void File::write()
    {
        if (!m_isOpen)
            throw std::runtime_error("File is not open");

        ::fwrite(m_buffer.data().get(), 1, m_size, m_file);
    }

} // namespace eutil