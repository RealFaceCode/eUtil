#include "bitset.hpp"

namespace eutils
{
    Bitset::Bitset()
    {
    }

    Bitset::Bitset(uint32_t data)
        : m_data(data)
    {
    }

    void Bitset::set(uint32_t index, bool bit)
    {
        bit ? m_data |= (1 << index) : m_data &= ~(1 << index);
    }

    bool Bitset::get(uint32_t index) const
    {
        return (m_data >> index) & 1;
    }
}