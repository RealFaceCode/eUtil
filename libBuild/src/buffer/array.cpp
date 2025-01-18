#include "eutil/buffer/array.hpp"

#include <memory>
#include <unordered_map>
#include <functional>
#include <string>
#include <cstring>

namespace util
{
    std::unordered_map<std::string, std::function<bool(void*, Array&)>> WriteRules;
    std::unordered_map<std::string, std::function<bool(void*, Array&)>> ReadRules;

    EUTIL_API Array CreateArray(size_t capacityInBytes)
    {
        Array array;
        array.data = new uint8_t[capacityInBytes];
        array.capacity = capacityInBytes;
        ZeroArray(array);
        return array;
    }

    EUTIL_API void FreeArray(Array& array)
    {
        delete[] array.data;
        array.data = nullptr;
        array.size = 0;
        array.capacity = 0;
        array.readOffset = 0;
        array.writeOffset = 0;
    }

    EUTIL_API void ResizeArray(Array& array, size_t newCapacityInBytes)
    {
        uint8_t* newData = new uint8_t[newCapacityInBytes];
        ZeroArray(array);
        memcpy(newData, array.data, array.size);
        delete[] array.data;
        array.data = newData;
        array.capacity = newCapacityInBytes;
    }

    EUTIL_API void WriteToArray(Array& array, const uint8_t* data, size_t size)
    {
        if(array.size + size > array.capacity)
            ResizeArray(array, array.capacity + size);

        memcpy(array.data + array.writeOffset, data, size);
        array.writeOffset += size;
        array.size += size;
    }

    EUTIL_API void WriteToArray(Array& array, void* data, size_t size)
    {
        WriteToArray(array, reinterpret_cast<const uint8_t*>(data), size);
    }

    EUTIL_API void ReadFromArray(Array& array, uint8_t* data, size_t size)
    {
        if(array.readOffset + size > array.size)
            return;

        memcpy(data, array.data + array.readOffset, size);
        array.readOffset += size;
    }

    EUTIL_API void ReadFromArray(Array& array, void* data, size_t size)
    {
        ReadFromArray(array, reinterpret_cast<uint8_t*>(data), size);
    }

    EUTIL_API void SetArrayReadOffset(Array& array, size_t offset)
    {
        array.readOffset = offset;
    }

    EUTIL_API void SetArrayWriteOffset(Array& array, size_t offset)
    {
        array.writeOffset = offset;
    }

    EUTIL_API void ResetArray(Array& array)
    {
        array.size = 0;
        array.readOffset = 0;
        array.writeOffset = 0;
    }

    EUTIL_API void ZeroArray(Array& array)
    {
        memset(array.data + array.size, 0, array.capacity - array.size);
    }

    EUTIL_API std::unordered_map<std::string, std::function<bool(void*, Array&)>>& GetWriteRules()
    {
        return WriteRules;
    }

    EUTIL_API std::unordered_map<std::string, std::function<bool(void*, Array&)>>& GetReadRules()
    {
        return ReadRules;
    }
}