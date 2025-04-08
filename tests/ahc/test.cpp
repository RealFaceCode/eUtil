#include "../test_util.hpp"
#include "eutil/buffer/array.hpp"

int main()
{
    // Test code
    util::Array array(1024);
    array.write("Hello, World!");
    array.write(42);
    array.write(std::vector<std::string>{"arg1", "arg2"});

    std::string readArray = array.read<std::string>();
    int readInt = array.read<int>();
    std::vector<std::string> readArgs = array.read<std::vector<std::string>>();

    compare("Hello, World!", readArray, "ahc Array test");
    compare(42, readInt, "ahc Array test");
    compare(std::vector<std::string>{"arg1", "arg2"}, readArgs, "ahc Array test");

    return ::FAILED;
}