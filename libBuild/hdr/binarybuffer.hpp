#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <memory>
#include <concepts>
#include <type_traits>

#include "defines.hpp"
#include "buffer.hpp"

namespace eutil 
{
    template <typename T>
    concept IsChar = std::is_same_v<T, char> || std::is_same_v<T, uint8_t> || std::is_same_v<T, wchar_t> || std::is_same_v<T, char16_t> || std::is_same_v<T, char32_t>;
    template <typename T>
    concept IsCharPtr = IsChar<T> && std::is_pointer_v<T>;
    template <typename T>
    concept IsString = std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring> || std::is_same_v<T, std::u8string> || std::is_same_v<T, std::u16string> || std::is_same_v<T, std::u32string>;
    template <typename T>
    concept IsStringView = std::is_same_v<T, std::string_view> || std::is_same_v<T, std::wstring_view> || std::is_same_v<T, std::u8string_view> || std::is_same_v<T, std::u16string_view> || std::is_same_v<T, std::u32string_view>;
    template <typename T>
    concept IsStringRepresentable = IsCharPtr<T> || IsString<T> || IsStringView<T>;

    template <typename T>
    size_t constexpr GetStringRepTypeSize()
    {
        if constexpr (IsChar<T> || IsCharPtr<T>)
            return sizeof(T);
        else if constexpr (IsString<T> || IsStringView<T>)
            return sizeof(typename T::value_type);
    }

    struct EUTIL_API BinaryBuffer
    {
        BinaryBuffer() = default;
        ~BinaryBuffer() = default;

        template<typename T>
        void constexpr push(const T& data)
        {
            if constexpr (IsStringRepresentable<T>)
            {
                size_t len = 0;
                uint8_t* ptr = nullptr;

                if constexpr (IsCharPtr<T>)
                {
                    len = std::char_traits<T>::length(data);
                    ptr = reinterpret_cast<uint8_t*>(data);
                }
                else
                {
                    len = data.size();
                    ptr = reinterpret_cast<uint8_t*>(const_cast<typename T::value_type*>(data.data()));
                }
                
                push<uint64_t>(len);

                constexpr size_t type_size = GetStringRepTypeSize<T>();
                if(m_buffer.size + len > m_buffer.capacity)
                    m_buffer.reserve(len, type_size);

                std::memcpy(m_buffer.data + m_buffer.size, ptr, len * type_size);
                m_buffer.size += len * type_size;
            }
            else
            {
                constexpr size_t type_size = sizeof(T);
                if(m_buffer.size + type_size > m_buffer.capacity)
                    m_buffer.reserve(1, type_size);

                std::memcpy(m_buffer.data + m_buffer.size, &data, type_size);
                m_buffer.size += type_size;
            }
        }

        template<typename T>
        T constexpr get()
        {
            if constexpr (IsStringRepresentable<T>)
            {
                constexpr size_t type_size = GetStringRepTypeSize<T>();
                auto len = get<uint64_t>();
                T temp;
                
                if constexpr (IsCharPtr<T>)
                {
                    temp = new std::remove_pointer_t<T>[len + 1];
                    std::memcpy(temp, m_buffer.data + m_readoffset, len * type_size);
                    temp[len] = '\0';
                }
                else
                {
                    temp.resize(len);
                    std::memcpy(temp.data(), m_buffer.data + m_readoffset, len * type_size);
                }

                m_readoffset += len * type_size;
                return temp;
            }
            else
            {
                constexpr size_t type_size = sizeof(T);
                T temp;
                std::memcpy(&temp, m_buffer.data + m_readoffset, type_size);
                m_readoffset += type_size;
                return temp;
            }
        }

    size_t constexpr size() const { return m_buffer.size; }
    size_t constexpr capacity() const { return m_buffer.capacity; }
    size_t constexpr readoffset() const { return m_readoffset; }
    const uint8_t* data() const { return m_buffer.data; }

    void reserve(size_t size, size_t typeSize = 1) { m_buffer.reserve(size, typeSize); }

    private:
        Buffer<uint8_t> m_buffer;
        size_t m_readoffset = 0;
    };
}
