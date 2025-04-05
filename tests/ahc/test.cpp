#include "../test_util.hpp"
#include "eutil/buffer/array.hpp"

int main()
{
    // Test code
    util::Array array(1024);
    array.write(std::string("Hello, World!"));
    array.setReadOffset(0);

    std::string readArray = array.read<std::string>();

    compare("Hello, World!", readArray, "ahc Array test");

    return ::FAILED;
}