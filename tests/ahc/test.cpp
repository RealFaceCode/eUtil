#include "../test_util.hpp"
#include "ahc.hpp"

int main()
{
    // Test code
    eutil::ahc::Array array = eutil::ahc::CreateArray(1024);
    eutil::ahc::WriteToArray(array, std::string("Hello, World!"));
    eutil::ahc::SetArrayReadOffset(array, 0);

    std::string readArray = eutil::ahc::ReadFromArray<std::string>(array);

    compare("Hello, World!", readArray, "ahc Array test");

    return ::FAILED;
}