#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <functional>
#include <typeinfo>
#include "eutil/defines.hpp"

namespace util
{
    template<typename T> concept IsString_v = std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring>;
    template<typename T> concept IsCharVector_v = std::is_same_v<T, std::vector<uint8_t>> || std::is_same_v<T, std::vector<std::byte>> || std::is_same_v<T, std::vector<char>>;
    template<typename T> concept IsChar_v = std::is_same_v<T, char> || std::is_same_v<T, wchar_t> || std::is_same_v<T, char16_t> || std::is_same_v<T, char32_t>;
    template<typename T> concept IsArithmetic_v = std::is_arithmetic_v<T> || std::is_enum_v<T>;
    template<typename T> concept IsArithmeticVector_v = std::is_same_v<T, std::vector<uint8_t>> || std::is_same_v<T, std::vector<std::byte>> || std::is_same_v<T, std::vector<char>> || std::is_same_v<T, std::vector<int>> || std::is_same_v<T, std::vector<float>> || std::is_same_v<T, std::vector<double>>;
    template<typename T> concept IsStringVector_v = std::is_same_v<T, std::vector<std::string>> || std::is_same_v<T, std::vector<std::wstring>>;
    template<typename T> concept IsCharPtr_v = std::is_pointer_v<T> && (std::is_same_v<typename std::remove_pointer_t<T>, char> || std::is_same_v<typename std::remove_pointer_t<T>, wchar_t> || std::is_same_v<typename std::remove_pointer_t<T>, char16_t> || std::is_same_v<typename std::remove_pointer_t<T>, char32_t>);
    template<typename T> concept IsConstCharPtr_v = std::is_pointer_v<T> && (std::is_same_v<typename std::remove_pointer_t<T>, const char> || std::is_same_v<typename std::remove_pointer_t<T>, const wchar_t> || std::is_same_v<typename std::remove_pointer_t<T>, const char16_t> || std::is_same_v<typename std::remove_pointer_t<T>, const char32_t>);
    
    struct EUTIL_API Array
    {
    public:
        Array() = default;
        explicit Array(size_t capacityInBytes);
        Array(const Array& other);
        Array(Array&& other) noexcept;
        ~Array();

        explicit operator bool() const;
        Array& operator=(const Array& other);
        Array& operator=(Array&& other) noexcept;

        void resize(size_t newCapacityInBytes);
        void write(const uint8_t* data, size_t wRize);
        void write(const void* data, size_t wRize);
        void read(uint8_t* data, size_t rSize);
        void read(void* data, size_t rSize);
        void setWriteOffset(size_t wOffset);
        void setReadOffset(size_t rOffset);
        void reset();
        void zero();
        void clear();

        template<typename T>
        void write(T data)
        {
            if constexpr(std::is_arithmetic_v<T>)
                write(&data, sizeof(T));
            else if constexpr(IsString_v<T>)
            {
                size_t length = data.size();
                size_t wSize = std::is_same_v<T, std::string> ? length : length * sizeof(wchar_t);
                write<size_t>(length);
                write(data.data(), wSize);
            }
            else if constexpr(IsCharVector_v<T>)
            {
                size_t length = data.size();
                write<size_t>(length);
                write(data.data(), length);
            }
            else if constexpr(IsChar_v<T>)
            {
                write(&data, sizeof(T));
            }
            else if constexpr(IsArithmetic_v<T>)
            {
                write(&data, sizeof(T));
            }
            else if constexpr(IsArithmeticVector_v<T>)
            {
                size_t length = data.size();
                write<size_t>(length);
                write(data.data(), length * sizeof(typename T::value_type));
            }
            else if constexpr(IsStringVector_v<T>)
            {
                size_t length = data.size();
                write<size_t>(length);
                for(const auto& str : data)
                {
                    size_t strLength = str.size();
                    size_t wSize = std::is_same_v<typename T::value_type, std::string> ? strLength : strLength * sizeof(wchar_t);
                    write<size_t>(strLength);
                    write(str.data(), wSize);
                }
            }
            else if constexpr(IsCharPtr_v<T> || IsConstCharPtr_v<T>)
            {
                size_t length = std::char_traits<typename std::remove_pointer_t<T>>::length(data);
                write<size_t>(length);
                write(data, length * sizeof(typename std::remove_pointer_t<T>));
            }
            else
                static_assert(false, "Unsupported type");
        }

        template<typename T>
        T read()
        {
            if constexpr(std::is_arithmetic_v<T>)
            {
                T data;
                read(&data, sizeof(T));
                return data;
            }
            else if constexpr(IsString_v<T>)
            {
                T data;
                size_t length = read<size_t>();
                size_t rSize = std::is_same_v<T, std::string> ? length : length * sizeof(wchar_t);
                data.resize(length);
                read(data.data(), rSize);
                return data;
            }
            else if constexpr(IsCharVector_v<T>)
            {
                T data;
                size_t length = Read<size_t>();
                data.resize(length);
                read(data.data(), length);
                return data;
            }
            else if constexpr(IsChar_v<T>)
            {
                T data;
                read(&data, sizeof(T));
                return data;
            }
            else if constexpr(IsArithmetic_v<T>)
            {
                T data;
                read(&data, sizeof(T));
                return data;
            }
            else if constexpr(IsArithmeticVector_v<T>)
            {
                T data;
                size_t length = Read<size_t>();
                data.resize(length);
                read(data.data(), length * sizeof(typename T::value_type));
                return data;
            }
            else if constexpr(IsStringVector_v<T>)
            {
                T data;
                size_t length = read<size_t>();
                data.resize(length);

                for(auto& str : data)
                {
                    size_t strLength = read<size_t>();
                    size_t rSize = std::is_same_v<typename T::value_type, std::string> ? strLength : strLength * sizeof(wchar_t);
                    str.resize(strLength);
                    read(str.data(), rSize);
                }

                return data;
            }
            else if constexpr(IsCharPtr_v<T> || IsConstCharPtr_v<T>)
            {
                size_t length = read<size_t>();
                T data = new typename std::remove_pointer_t<T>[length + 1];
                read(data, length * sizeof(typename std::remove_pointer_t<T>));
                data[length] = '\0';
                return data;
            }
            else
                static_assert(false, "Unsupported type");
        }

        template<typename T>
        friend Array& operator<<(Array& array, const T& data)
        {
            array.write(data);
            return array;
        }

        template<typename T>
        friend Array& operator>>(Array& array, T& data)
        {
            data = array.read<T>();
            return array;
        }

        template<typename T>
        bool writeRule(T& data)
        {
            std::string key = typeid(T).name();
            auto it = WriteRules.find(key);
            if(it == WriteRules.end())
                return false;

            return it->second(&data, *this);
        }

        template<typename T>
        T readRule()
        {
            std::string key = typeid(T).name();
            auto it = ReadRules.find(key);
            if(it == ReadRules.end())
                return T{};

            T data;
            if(!it->second(&data, *this))
                return T{};

            return data;
        }

        template<typename T>
        static void AddWriteRule(const std::function<bool(void*, Array&)>& rule)
        {
            std::string key = typeid(T).name();
            WriteRules[key] = rule;
        }

        template<typename T>
        static void AddReadRule(const std::function<bool(void*, Array&)>& rule)
        {
            std::string key = typeid(T).name();
            ReadRules[key] = rule;
        }
        template<typename T>
        static bool RemoveWriteRule()
        {
            std::string key = typeid(T).name();
            auto it = WriteRules.find(key);
            if(it == WriteRules.end())
                return false;

            WriteRules.erase(it);
            return true;
        }

        template<typename T>
        static bool RemoveReadRule(const std::string& key)
        {
            auto it = ReadRules.find(key);
            if(it == ReadRules.end())
                return false;

            ReadRules.erase(it);
            return true;
        }

        uint8_t* ptr();
        const uint8_t* data() const;
        size_t capacity() const;
        size_t size() const;
        size_t writeOffset() const;
        size_t readOffset() const;

        static std::unordered_map<std::string, std::function<bool(void*, Array&)>>& GetWriteRules();
        static std::unordered_map<std::string, std::function<bool(void*, Array&)>>& GetReadRules();

    private:
        uint8_t* arrData = nullptr;
        size_t arrSize = 0;
        size_t arrCapacity = 0;
        size_t rOffset = 0;
        size_t wOffset = 0;

        static std::unordered_map<std::string, std::function<bool(void*, Array&)>> WriteRules;
        static std::unordered_map<std::string, std::function<bool(void*, Array&)>> ReadRules;
    };
}