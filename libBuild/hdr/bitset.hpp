#pragma once

#include <cstdint>

namespace eutils
{
    struct Bitset
    {
    public:
        Bitset();
        explicit Bitset(uint32_t data);

        void set(uint32_t index, bool bit);
        bool get(uint32_t index) const;

    private:
        uint32_t m_data = 0;
    };
}