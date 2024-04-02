#include "bitset.hpp"

namespace eutils
{
    EUTIL_API Bitset::Bitset() = default;

    EUTIL_API Bitset::Bitset(uint32_t data)
        : m_data(data)
    {
    }

    EUTIL_API void Bitset::set(uint32_t index, bool bit)
    {
        bit ? m_data |= (1 << index) : m_data &= ~(1 << index);
    }

    EUTIL_API bool Bitset::get(uint32_t index) const
    {
        return (m_data >> index) & 1;
    }
}