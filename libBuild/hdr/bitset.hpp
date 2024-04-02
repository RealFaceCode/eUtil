#pragma once

#include <cstdint>

#include "defines.hpp"

namespace eutils
{
    struct EUTIL_API Bitset
    {
    public:
        EUTIL_API Bitset();
        EUTIL_API explicit Bitset(uint32_t data);

        EUTIL_API void set(uint32_t index, bool bit);
        EUTIL_API bool get(uint32_t index) const;

    private:
        uint32_t m_data = 0;
    };
}