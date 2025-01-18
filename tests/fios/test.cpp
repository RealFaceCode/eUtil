#include "../test_util.hpp"
#include "eutil/fios.hpp"

int main()
{
    // Test code
    eutil::fios::WriteDataToFile("test.txt", reinterpret_cast<const uint8_t*>("Hello, World!"), 13);

    size_t size = 0;
    auto buffer = eutil::fios::ReadDataFromFile("test.txt", size, true);

    std::string readData((char*)(buffer));

    compare("Hello, World!", readData, "fios File I/O test");

    free(buffer);

    return ::FAILED;
}