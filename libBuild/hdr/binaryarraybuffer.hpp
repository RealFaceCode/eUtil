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
        void push_back(const T& value, bool only_str = false);

        template<typename T>
        T getNextElement(bool only_str = false);

        std::shared_ptr<uint8_t[]> data(); 
        std::shared_ptr<const uint8_t[]> data() const;
    private:
        std::shared_ptr<uint8_t[]> m_data   = nullptr;
        size_t m_size                       = 0;
        size_t m_capacity                   = 0;
        size_t m_offset                     = 0;
    };
}
