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
        void write(void* data, size_t wRize);
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
                Write(&data, sizeof(T));
            else if constexpr(IsString_v<T>)
            {
                size_t length = data.size();
                size_t wSize = std::is_same_v<T, std::string> ? length : length * sizeof(wchar_t);
                Write<size_t>(length);
                Write(data.data(), wSize);
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
                Read(&data, sizeof(T));
                return data;
            }
            else if constexpr(IsString_v<T>)
            {
                T data;
                size_t length = Read<size_t>();
                size_t rSize = std::is_same_v<T, std::string> ? length : length * sizeof(wchar_t);
                data.resize(length);
                Read(data.data(), rSize);
                return data;
            }
            else
                static_assert(false, "Unsupported type");
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