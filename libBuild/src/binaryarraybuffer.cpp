#include "binaryarraybuffer.hpp"

#include <type_traits>
#include <algorithm>
#include <cstring>

namespace eutil
{
    EUTIL_API BinaryArrayBuffer::BinaryArrayBuffer() = default;

    EUTIL_API void BinaryArrayBuffer::reserve(size_t size)
    {
        if (m_data == nullptr)
        {
            m_data = std::make_unique<uint8_t[]>(size);
            m_capacity = size;
        }
        else
        {
            auto newCapacity = m_capacity + size;
            auto new_data = std::make_unique<uint8_t[]>(newCapacity);
            ::memcpy(new_data.get(), m_data.get(), m_size);
            m_data = std::move(new_data);
            m_capacity += size;
        }
    }

    EUTIL_API void BinaryArrayBuffer::resize(size_t size)
    {
        if (size > m_capacity)
        {
            reserve(size - m_capacity);
        }
        m_size = size;
    }

    EUTIL_API void BinaryArrayBuffer::clear()
    {
        std::fill(m_data.get(), m_data.get() + m_size, 0);
        m_size = 0;
        m_offset = 0;
    }

    EUTIL_API size_t BinaryArrayBuffer::size() const
    {
        return m_size;
    }

    EUTIL_API size_t BinaryArrayBuffer::capacity() const
    {
        return m_capacity;
    }

    EUTIL_API bool BinaryArrayBuffer::empty() const
    {
        return m_size == 0;
    }

    EUTIL_API void BinaryArrayBuffer::shrink_to_fit()
    {
        if (m_capacity > m_size)
        {
            auto new_data = std::make_unique<uint8_t[]>(m_size);
            ::memcpy(new_data.get(), m_data.get(), m_size);
            m_data = std::move(new_data);
            m_capacity = m_size;
        }
    }

    EUTIL_API void BinaryArrayBuffer::push_rawData(const uint8_t* value, size_t size)
    {
        if (m_capacity < m_size + size)
        {
            auto newSize = m_size + size - m_capacity;
            reserve(newSize);
        }

        ::memcpy(m_data.get() + m_size, value, size);
        m_size += size;
    }

    template<typename T>
    EUTIL_API void BinaryArrayBuffer::push_back(const T& value, bool only_str)
    {
        if constexpr (  std::is_same_v<T, std::string> ||
                        std::is_same_v<T, std::string_view> ||
                        std::is_same_v<T, const std::string&> ||
                        std::is_same_v<T, std::wstring> ||
                        std::is_same_v<T, std::wstring_view> ||
                        std::is_same_v<T, const std::wstring&>)
        {
            auto size = value.size() * sizeof(value[0]);
            if(!only_str) 
                push_back<uint64_t>(value.size());

            if (m_capacity < m_size + size)
            {
                auto rSize = m_size + size - m_capacity;
                reserve(rSize);
            }

            ::memcpy(m_data.get() + m_size, value.data(), size);
            m_size += size;
        }
        else
        {
            constexpr auto size = sizeof(T);
            if (m_capacity < m_size + size)
            {
                auto newSize = m_size + size - m_capacity;
                reserve(newSize);
            }

            ::memcpy(m_data.get() + m_size, &value, size);
            m_size += size;
        }    
    }

    template EUTIL_API void BinaryArrayBuffer::push_back<uint8_t>(const uint8_t& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<uint16_t>(const uint16_t& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<uint32_t>(const uint32_t& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<uint64_t>(const uint64_t& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<int8_t>(const int8_t& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<int16_t>(const int16_t& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<int32_t>(const int32_t& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<int64_t>(const int64_t& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<float>(const float& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<double>(const double& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<bool>(const bool& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<char>(const char& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<wchar_t>(const wchar_t& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<char16_t>(const char16_t& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<char32_t>(const char32_t& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<std::string>(const std::string& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<std::string_view>(const std::string_view& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<const std::string&>(const std::string& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<std::wstring>(const std::wstring& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<std::wstring_view>(const std::wstring_view& value, bool only_str);
    template EUTIL_API void BinaryArrayBuffer::push_back<const std::wstring&>(const std::wstring& value, bool only_str);


    template<typename T>
    EUTIL_API T BinaryArrayBuffer::getNextElement(bool only_str)
    {
        if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring>)
        {
            T value;
            auto size = only_str ? m_size :getNextElement<uint64_t>();
            const uint8_t* data = m_data.get() + m_offset;
            value.resize(size);
            ::memcpy(value.data(), data, size * sizeof(value[0]));
            m_offset += size * sizeof(value[0]);
            return value;
        }
        else
        {
            constexpr auto size = sizeof(T);
            T value;
            ::memcpy(&value, m_data.get() + m_offset, size);
            m_offset += size;
            return value;
        }
    }

    template EUTIL_API uint8_t BinaryArrayBuffer::getNextElement<uint8_t>(bool only_str);
    template EUTIL_API uint16_t BinaryArrayBuffer::getNextElement<uint16_t>(bool only_str);
    template EUTIL_API uint32_t BinaryArrayBuffer::getNextElement<uint32_t>(bool only_str);
    template EUTIL_API uint64_t BinaryArrayBuffer::getNextElement<uint64_t>(bool only_str);
    template EUTIL_API int8_t BinaryArrayBuffer::getNextElement<int8_t>(bool only_str);
    template EUTIL_API int16_t BinaryArrayBuffer::getNextElement<int16_t>(bool only_str);
    template EUTIL_API int32_t BinaryArrayBuffer::getNextElement<int32_t>(bool only_str);
    template EUTIL_API int64_t BinaryArrayBuffer::getNextElement<int64_t>(bool only_str);
    template EUTIL_API float BinaryArrayBuffer::getNextElement<float>(bool only_str);
    template EUTIL_API double BinaryArrayBuffer::getNextElement<double>(bool only_str);
    template EUTIL_API bool BinaryArrayBuffer::getNextElement<bool>(bool only_str);
    template EUTIL_API char BinaryArrayBuffer::getNextElement<char>(bool only_str);
    template EUTIL_API wchar_t BinaryArrayBuffer::getNextElement<wchar_t>(bool only_str);
    template EUTIL_API char16_t BinaryArrayBuffer::getNextElement<char16_t>(bool only_str);
    template EUTIL_API char32_t BinaryArrayBuffer::getNextElement<char32_t>(bool only_str);
    template EUTIL_API std::string BinaryArrayBuffer::getNextElement<std::string>(bool only_str);
    template EUTIL_API std::wstring BinaryArrayBuffer::getNextElement<std::wstring>(bool only_str);

    EUTIL_API std::shared_ptr<uint8_t[]> BinaryArrayBuffer::data()
    {
        return m_data;
    }

    EUTIL_API std::shared_ptr<const uint8_t[]> BinaryArrayBuffer::data() const
    {
        return m_data;
    }
}