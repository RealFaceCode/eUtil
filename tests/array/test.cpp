#include "../test_util.hpp"
#include "eutil/buffer/Array.hpp"

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

    array2 >> readString;

    ::compare("array test", readString, "Failed << >> operators");

    // Test code

    struct TestStruct
    {
        int a;
        int b;
    };

    util::Array::AddReadRule<TestStruct>([](void* data, util::Array& array) -> bool
    {
        auto* test = static_cast<TestStruct*>(data);
        array.read(test, sizeof(int) * 2);
        return true;
    });

    util::Array::AddWriteRule<TestStruct>([](const void* data, util::Array& array) -> bool
    {
        const auto* test = static_cast<const TestStruct*>(data);
        array.write(test, sizeof(int) * 2);
        return true;
    });

    TestStruct testStruct = {123, 456};
    TestStruct readTestStruct;

    util::Array array3(1024);

    array3 << testStruct;
    array3 >> readTestStruct;

    ::compare(testStruct.a, readTestStruct.a, "Failed Struct");
    ::compare(testStruct.b, readTestStruct.b, "Failed Struct");

    return ::FAILED;
}