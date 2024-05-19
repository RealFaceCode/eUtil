#pragma once

#include "defines.hpp"

namespace eutil
{
    template <typename T>
    struct EUTIL_API Buffer
    {
        Buffer() = default;

        ~Buffer()
        {
            if(data != nullptr)
            {
            }
        }

        void reserve(size_t elements, size_t typeSize)
        {
            size_t bytes = elements * typeSize;
            if(data == nullptr)
            {
                data = static_cast<T*>(malloc(bytes));
                capacity = bytes;
            }
            else
            {
                size_t newCapacity = capacity + bytes;
                data = static_cast<T*>(realloc(data, newCapacity));
                capacity = newCapacity;
            }
        }

        T* data         = nullptr;
        size_t size     = 0;
        size_t capacity = 0;

    private:
        size_t type_size = sizeof(T);
    };
}