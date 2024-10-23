#include "../test_util.hpp"

#include "fioc.hpp"

int main()
{
    // Test code
    if(!eutil::fioc::FileExists("test.txt"))
        eutil::fioc::CreateFile("test.txt");

    FILE* file = nullptr;

    std::string data = "Hello, World!";
    eutil::fioc::WriteDataToFileRaw(&file, "test.txt", data.data(), data.size(), true);

    size_t size = 0;
    void* buffer = nullptr;
    eutil::fioc::ReadDataFromFileRaw(&file, "test.txt", &buffer, data.size(), data.size() + 1, true);
    char* bufferChar = (char*)(buffer);
    bufferChar[data.size()] = '\0';
    std::string readData((char*)(bufferChar));

    compare(data, readData, "fioc File I/O test");

    free(buffer);
    return ::FAILED;
}