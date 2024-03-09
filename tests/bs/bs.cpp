#include "../test_util.hpp"
#include "bitset.hpp"

int main()
{
    eutils::Bitset bitset;
    bitset.set(0, true);
    bitset.set(1, false);
    bitset.set(2, true);
    bitset.set(3, false);
    bitset.set(4, true);
    bitset.set(5, false);
    bitset.set(6, true);
    bitset.set(7, false);
    bitset.set(8, true);
    bitset.set(9, false);
    bitset.set(10, true);
    bitset.set(11, false);
    bitset.set(12, true);
    bitset.set(13, false);
    bitset.set(14, true);
    bitset.set(15, false);
    bitset.set(16, true);
    bitset.set(17, false);
    bitset.set(18, true);
    bitset.set(19, false);
    bitset.set(20, true);
    bitset.set(21, false);
    bitset.set(22, true);
    bitset.set(23, false);
    bitset.set(24, true);
    bitset.set(25, false);
    bitset.set(26, true);
    bitset.set(27, false);
    bitset.set(28, true);
    bitset.set(29, false);
    bitset.set(30, true);
    bitset.set(31, false);
    
    compare(true, bitset.get(0), "Bit 0");
    compare(false, bitset.get(1), "Bit 1");
    compare(true, bitset.get(2), "Bit 2");
    compare(false, bitset.get(3), "Bit 3");
    compare(true, bitset.get(4), "Bit 4");
    compare(false, bitset.get(5), "Bit 5");
    compare(true, bitset.get(6), "Bit 6");
    compare(false, bitset.get(7), "Bit 7");
    compare(true, bitset.get(8), "Bit 8");
    compare(false, bitset.get(9), "Bit 9");
    compare(true, bitset.get(10), "Bit 10");
    compare(false, bitset.get(11), "Bit 11");
    compare(true, bitset.get(12), "Bit 12");
    compare(false, bitset.get(13), "Bit 13");
    compare(true, bitset.get(14), "Bit 14");
    compare(false, bitset.get(15), "Bit 15");
    compare(true, bitset.get(16), "Bit 16");
    compare(false, bitset.get(17), "Bit 17");
    compare(true, bitset.get(18), "Bit 18");
    compare(false, bitset.get(19), "Bit 19");
    compare(true, bitset.get(20), "Bit 20");
    compare(false, bitset.get(21), "Bit 21");
    compare(true, bitset.get(22), "Bit 22");
    compare(false, bitset.get(23), "Bit 23");
    compare(true, bitset.get(24), "Bit 24");
    compare(false, bitset.get(25), "Bit 25");
    compare(true, bitset.get(26), "Bit 26");
    compare(false, bitset.get(27), "Bit 27");
    compare(true, bitset.get(28), "Bit 28");
    compare(false, bitset.get(29), "Bit 29");
    compare(true, bitset.get(30), "Bit 30");
    compare(false, bitset.get(31), "Bit 31");
    
    return ::FAILED;
}