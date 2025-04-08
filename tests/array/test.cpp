#include "../test_util.hpp"
#include "eutil/buffer/array.hpp"

int main()
{
    // Test code
    util::Array array(1024);
    array.write("Hello, World!");
    array.write(42);
    std::vector<std::string> argVec = {"arg1", "arg2"};
    array.write(argVec);

    std::string readArray = array.read<std::string>();
    int readInt = array.read<int>();
    std::vector<std::string> readArgs = array.read<std::vector<std::string>>();

    ::compare("Hello, World!", readArray, "Failed String");
    ::compare(42, readInt, "Failed Numeric");
    ::compare(argVec[0], readArgs[0], "Failed Vector");
    ::compare(argVec[1], readArgs[1], "Failed Vector");

    // Test code
    util::Array array2(1024);
    array2 << "array test";
    
    std::string readString;
    array >> readString;

    ::compare("array test", readString, "Failed << >> operators");

    return ::FAILED;
}