#include "../test_util.hpp"
#include "eutil/buffer/ahc.hpp"

int main()
{
    // Test code
    util::Array array = util::CreateArray(1024);
    util::WriteToArray(array, std::string("Hello, World!"));
    util::SetArrayReadOffset(array, 0);

    std::string readArray = util::ReadFromArray<std::string>(array);

    compare("Hello, World!", readArray, "ahc Array test");

    return ::FAILED;
}