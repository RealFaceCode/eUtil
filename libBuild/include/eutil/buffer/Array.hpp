#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <functional>
#include <typeinfo>
#include <optional>
#include <assert.h>
#include <print>

#include "eutil/defines.hpp"
#include "eutil/conceps.hpp"

namespace util
{
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

        bool readFrom(std::fstream& file, size_t wSize);

        template<typename T>
        void write(const T& data)
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
            else if constexpr(IsCharArray_v<T> || IsConstCharArray_v<T>)
            {
                size_t length = std::char_traits<typename std::remove_extent_t<T>>::length(data);
                write<size_t>(length);
                write(data, length * sizeof(typename std::remove_extent_t<T>));
            }
            else
            {
                auto result = writeRule(data);
                if(!result)
                {
                    std::string key = typeid(T).name();
                    std::print("{}{}\n", "Unsupported type: ", key);
                    std::print("Please add a write rule for this type.\n");
                    std::print("Example: Array::AddWriteRule<YourType>([](void* data, Array& array) -> bool {{\n");
                    std::print("    YourType* yourData = static_cast<YourType*>(data);\n");
                    std::print("    array.write(yourData, sizeof(YourType));\n");
                    std::print("    return true;\n");
                    std::print("}});\n");
                    std::print("Or use a supported type.\n");

                    std::print("Type: {}\n", typeid(T).name());
                    std::print("Size: {}\n", sizeof(T));

                    assert(false);
                }
            }
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
            {
                auto result = readRule<T>();
                if(result.has_value())
                    return result.value();
                else
                {
                    std::string key = typeid(T).name();
                    std::print("{}{}\n", "Unsupported type: ", key);
                    std::print("Please add a read rule for this type.\n");
                    std::print("Example: Array::AddReadRule<YourType>([](void* data, Array& array) -> bool {{\n");
                    std::print("    YourType* yourData = static_cast<YourType*>(data);\n");
                    std::print("    array.read(yourData, sizeof(YourType));\n");
                    std::print("    return true;\n");
                    std::print("}});\n");
                    std::print("Or use a supported type.\n");

                    std::print("Type: {}\n", typeid(T).name());
                    std::print("Size: {}\n", sizeof(T));

                    assert(false);
                    return T{}; // Return default value if type is not supported
                }
            }
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
        bool writeRule(const T& data)
        {
            std::string key = typeid(T).name();
            auto it = WriteRules.find(key);
            if(it == WriteRules.end())
                return false;

            return it->second(&data, *this);
        }

        template<typename T>
        std::optional<T> readRule()
        {
            std::string key = typeid(T).name();
            auto it = ReadRules.find(key);
            if(it == ReadRules.end())
                return std::nullopt;

            T data;
            if(!it->second(&data, *this))
                return std::nullopt;

            return data;
        }

        template<typename T>
        static void AddWriteRule(const std::function<bool(const void*, Array&)>& rule)
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
        size_t remainingBytes() const;
        size_t remainingBytesWrite() const;
        size_t remainingBytesRead() const;
        bool isEmpty() const;
        bool isFull() const;

        static std::unordered_map<std::string, std::function<bool(const void*, Array&)>>& GetWriteRules();
        static std::unordered_map<std::string, std::function<bool(void*, Array&)>>& GetReadRules();

    private:
        uint8_t* arrData = nullptr;
        size_t arrSize = 0;
        size_t arrCapacity = 0;
        size_t rOffset = 0;
        size_t wOffset = 0;

        static std::unordered_map<std::string, std::function<bool(const void*, Array&)>> WriteRules;
        static std::unordered_map<std::string, std::function<bool(void*, Array&)>> ReadRules;
    };
}