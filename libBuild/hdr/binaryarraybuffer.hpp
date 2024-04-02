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
        EUTIL_API BinaryArrayBuffer();

        EUTIL_API void reserve(size_t size);
        EUTIL_API void resize(size_t size);
        EUTIL_API void clear();

        EUTIL_API size_t size() const;
        EUTIL_API size_t capacity() const;
        EUTIL_API bool empty() const;

        EUTIL_API void shrink_to_fit();

        EUTIL_API void push_rawData(const uint8_t* value, size_t size);

        template<typename T>
        EUTIL_API void push_back(const T& value, bool only_str = false);

        template<typename T>
        EUTIL_API T getNextElement(bool only_str = false);

        EUTIL_API std::shared_ptr<uint8_t[]> data(); 
        EUTIL_API std::shared_ptr<const uint8_t[]> data() const;
    private:
        std::shared_ptr<uint8_t[]> m_data   = nullptr;
        size_t m_size                       = 0;
        size_t m_capacity                   = 0;
        size_t m_offset                     = 0;
    };
}
