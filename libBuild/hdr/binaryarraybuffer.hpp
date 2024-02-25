#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <memory>

namespace eutil 
{
    struct BinaryArrayBuffer
    {
    public:
        BinaryArrayBuffer();

        void reserve(size_t size);
        void resize(size_t size);
        void clear();

        size_t size() const;
        size_t capacity() const;
        bool empty() const;

        void shrink_to_fit();

        template<typename T>
        void push_back(const T& value);

        template<typename T>
        T getNextElement();

        std::shared_ptr<uint8_t[]> data(); 
        std::shared_ptr<const uint8_t[]> data() const;
    private:
        std::shared_ptr<uint8_t[]> m_data   = nullptr;
        size_t m_size                       = 0;
        size_t m_capacity                   = 0;
        size_t m_offset                     = 0;
    };
}
