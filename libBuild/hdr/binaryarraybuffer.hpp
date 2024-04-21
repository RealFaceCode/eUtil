#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <memory>

#include "defines.hpp"

namespace eutil 
{
    struct EUTIL_API BinaryArrayBuffer
    {
    public:
        BinaryArrayBuffer() = default;

        void reserve(size_t size);
        void resize(size_t size);
        void clear();

        size_t size() const;
        size_t capacity() const;
        bool empty() const;

        void shrink_to_fit();

        void push_rawData(const uint8_t* value, size_t size);

        template<typename T>
        void push_back(const T& value, bool only_str = false)
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

        template<typename T>
        T getNextElement(bool only_str = false)
        {
            if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring>)
            {
                T value;
                auto size = only_str ? m_size : getNextElement<uint64_t>();
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

        std::shared_ptr<uint8_t[]> data(); 
        std::shared_ptr<const uint8_t[]> data() const;
    private:
        std::shared_ptr<uint8_t[]> m_data   = nullptr;
        size_t m_size                       = 0;
        size_t m_capacity                   = 0;
        size_t m_offset                     = 0;
    };
}
