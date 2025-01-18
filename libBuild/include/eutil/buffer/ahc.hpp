#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <functional>
#include <typeinfo>
#include "eutil/defines.hpp"

namespace eutil::ahc
{

    template<typename T> concept IsString_v = std::is_same_v<T, std::string> || std::is_same_v<T, std::wstring>;

    struct EUTIL_API Array
    {
    public:
        uint8_t* data       = nullptr;
        size_t size         = 0;
        size_t capacity     = 0;
        size_t readOffset   = 0;
        size_t writeOffset  = 0;
    };

    EUTIL_API Array CreateArray(size_t capacityInBytes);
    EUTIL_API void FreeArray(Array& array);
    EUTIL_API void ResizeArray(Array& array, size_t newCapacityInBytes);
    EUTIL_API void WriteToArray(Array& array, const uint8_t* data, size_t size);
    EUTIL_API void WriteToArray(Array& array, void* data, size_t size);
    EUTIL_API void ReadFromArray(Array& array, uint8_t* data, size_t size);
    EUTIL_API void ReadFromArray(Array& array, void* data, size_t size);
    EUTIL_API void SetArrayReadOffset(Array& array, size_t offset);
    EUTIL_API void SetArrayWriteOffset(Array& array, size_t offset);
    EUTIL_API void ResetArray(Array& array);
    EUTIL_API void ZeroArray(Array& array);

    template<typename T>
    void WriteToArray(Array& array, T data)
    {
        if constexpr(std::is_arithmetic_v<T>)
            WriteToArray(array, &data, sizeof(T));
        else if constexpr(IsString_v<T>)
        {
            size_t length = data.size();
            size_t size = std::is_same_v<T, std::string> ? length : length * sizeof(wchar_t);
            WriteToArray<size_t>(array, length);
            WriteToArray(array, data.data(), size);
        }
        else
            static_assert(false, "Unsupported type");
    }

    template<typename T>
    T ReadFromArray(Array& array)
    {
        if constexpr(std::is_arithmetic_v<T>)
        {
            T data;
            ReadFromArray(array, &data, sizeof(T));
            return data;
        }
        else if constexpr(IsString_v<T>)
        {
            T data;
            size_t length = ReadFromArray<size_t>(array);
            size_t size = std::is_same_v<T, std::string> ? length : length * sizeof(wchar_t);
            data.resize(length);
            ReadFromArray(array, data.data(), size);
            return data;
        }
        else
            static_assert(false, "Unsupported type");
    }

    EUTIL_API std::unordered_map<std::string, std::function<bool(void*, Array&)>>& GetWriteRules();
    EUTIL_API std::unordered_map<std::string, std::function<bool(void*, Array&)>>& GetReadRules();

    template<typename T>
    void AddWriteToArrayRule(bool (*rule)(void*, Array&))
    {
        std::string key = typeid(T).name();
        GetWriteRules()[key] = rule;
    }

    template<typename T>
    void AddReadFromArrayRule(bool (*rule)(void*, Array&))
    {
        std::string key = typeid(T).name();
        GetReadRules()[key] = rule;
    }

    template<typename T>
    bool WriteToArray(void* data, Array& array)
    {
        std::string key = typeid(T).name();
        auto WriteRules = GetWriteRules();
        auto it = WriteRules.find(key);
        if(it == WriteRules.end())
            return false;

        return it->second(data, array);
    }

    template<typename T>
    bool ReadFromArray(void* data, Array& array)
    {
        std::string key = typeid(T).name();
        auto ReadRules = GetReadRules();
        auto it = ReadRules.find(key);
        if(it == ReadRules.end())
            return false;

        return it->second(data, array);
    }
}// namespace ahc