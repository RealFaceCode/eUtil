#include "eutil/buffer/Array.hpp"

#include <memory>
#include <unordered_map>
#include <functional>
#include <string>
#include <cstring>
#include <fstream>

namespace util
{
    Array::Array(size_t capacityInBytes)
    {
        arrData = new uint8_t[capacityInBytes];
        arrCapacity = capacityInBytes;
        reset();
    }

    Array::Array(const Array& other)
    {
        arrData = new uint8_t[other.arrCapacity];
        arrCapacity = other.arrCapacity;
        arrSize = other.arrSize;
        wOffset = other.wOffset;
        rOffset = other.rOffset;
        ::memcpy(arrData, other.arrData, arrSize);
    }

    Array::Array(Array&& other) noexcept
    {
        arrData = other.arrData;
        arrCapacity = other.arrCapacity;
        arrSize = other.arrSize;
        wOffset = other.wOffset;
        rOffset = other.rOffset;
        other.arrData = nullptr;
        other.arrCapacity = 0;
        other.arrSize = 0;
        other.wOffset = 0;
        other.rOffset = 0;
    }

    Array::~Array()
    {
        delete[] arrData;
    }

    Array::operator bool() const
    {
        return arrData != nullptr;
    }

    Array& Array::operator=(const Array& other)
    {
        if(this != &other)
        {
            delete[] arrData;
            arrData = new uint8_t[other.arrCapacity];
            arrCapacity = other.arrCapacity;
            arrSize = other.arrSize;
            wOffset = other.wOffset;
            rOffset = other.rOffset;
            ::memcpy(arrData, other.arrData, arrSize);
        }

        return *this;
    }

    Array& Array::operator=(Array&& other) noexcept
    {
        if(this != &other)
        {
            delete[] arrData;
            arrData = other.arrData;
            arrCapacity = other.arrCapacity;
            arrSize = other.arrSize;
            wOffset = other.wOffset;
            rOffset = other.rOffset;
            other.arrData = nullptr;
            other.arrCapacity = 0;
            other.arrSize = 0;
            other.wOffset = 0;
            other.rOffset = 0;
        }

        return *this;
    }

    void Array::resize(size_t newCapacityInBytes)
    {
        uint8_t* newData = new uint8_t[newCapacityInBytes];
        ::memset(newData, 0, newCapacityInBytes);
        ::memcpy(newData, arrData, arrSize);
        delete[] arrData;
        arrData = newData;
        arrCapacity = newCapacityInBytes;
    }

    void Array::write(const uint8_t* data, size_t wSize)
    {
        if(arrSize + wSize > arrCapacity)
            resize(arrCapacity + wSize);

        ::memcpy(arrData + wOffset, data, wSize);
        wOffset += wSize;
        arrSize += wSize;
    }

    void Array::write(const void* data, size_t wSize)
    {
        write(reinterpret_cast<const uint8_t*>(data), wSize);
    }

    void Array::read(uint8_t* data, size_t rSize)
    {
        if(rOffset + rSize > arrSize)
            return;

        ::memcpy(data, arrData + rOffset, rSize);
        rOffset += rSize;
    }

    void Array::read(void* data, size_t rSize)
    {
        read(reinterpret_cast<uint8_t*>(data), rSize);
    }

    void Array::setWriteOffset(size_t writeOffset)
    {
        wOffset = writeOffset;
    }

    void Array::setReadOffset(size_t readOffset)
    {
        rOffset = readOffset;
    }

    void Array::reset()
    {
        arrSize = 0;
        wOffset = 0;
        rOffset = 0;
    }

    void Array::zero()
    {
        ::memset(arrData, 0, arrCapacity);
    }

    void Array::clear()
    {
        reset();
        zero();
    }

    bool Array::readFrom(std::fstream& file, size_t wSize)
    {
        if(!file.is_open())
            return false;

        file.read(reinterpret_cast<char*>(arrData + wOffset), wSize);
        if(!file.good())
            return false;

        arrSize += wSize;
        wOffset += wSize;

        return true;
    }

    uint8_t* Array::ptr()
    {
        return arrData;
    }

    const uint8_t* Array::data() const
    {
        return arrData;
    }

    size_t Array::capacity() const
    {
        return arrCapacity;
    }

    size_t Array::size() const
    {
        return arrSize;
    }

    size_t Array::writeOffset() const
    {
        return wOffset;
    }

    size_t Array::readOffset() const
    {
        return rOffset;
    }

    size_t Array::remainingBytes() const
    {
        return arrCapacity - arrSize;
    }

    size_t Array::remainingBytesWrite() const
    {
        return arrCapacity - wOffset;
    }

    size_t Array::remainingBytesRead() const
    {
        return arrSize - rOffset;
    }

    bool Array::isEmpty() const
    {
        return arrSize == 0;
    }

    bool Array::isFull() const
    {
        return arrSize == arrCapacity;
    }

    std::string Array::toString() const
    {
        std::string result;
        result.reserve(arrSize + 1);
        result.resize(arrSize + 1);

        auto src = reinterpret_cast<const char*>(arrData);
        auto dest = result.data();

        ::memcpy(dest, src, arrSize);
        dest[arrSize] = '\0';

        return result;
    }

    std::unordered_map<std::string, std::function<bool(const void*, Array&)>> Array::WriteRules;
    std::unordered_map<std::string, std::function<bool(void*, Array&)>> Array::ReadRules;

    std::unordered_map<std::string, std::function<bool(const void*, Array&)>>& Array::GetWriteRules()
    {
        return WriteRules;
    }

    std::unordered_map<std::string, std::function<bool(void*, Array&)>>& Array::GetReadRules()
    {
        return ReadRules;
    }
}