#include "binaryarraybuffer.hpp"

#include <type_traits>
#include <algorithm>
#include <cstring>

namespace eutil
{
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

    EUTIL_API std::shared_ptr<uint8_t[]> BinaryArrayBuffer::data()
    {
        return m_data;
    }

    EUTIL_API std::shared_ptr<const uint8_t[]> BinaryArrayBuffer::data() const
    {
        return m_data;
    }
}