#include "../test_util.hpp"

#include "eutil/filesystem/fileiocore.hpp"

int main()
{
    // Test code
    if(!util::FileExists("test.txt"))
        util::CreateFile("test.txt");

    FILE* file = nullptr;

    std::string data = "Hello, World!";
    util::WriteDataToFileRaw(&file, "test.txt", data.data(), data.size(), true);

    void* buffer = nullptr;
    util::ReadDataFromFileRaw(&file, "test.txt", &buffer, data.size(), data.size() + 1, true);
    char* bufferChar = (char*)(buffer);
    bufferChar[data.size()] = '\0';
    std::string readData((char*)(bufferChar));

    compare(data, readData, "fioc File I/O test");

    free(buffer);
    return ::FAILED;
}