#include "file.hpp"
#include "../test_util.hpp"
#include "eutil.hpp"
#include <fstream>

int main()
{
    std::filesystem::path path = "test.txt";

    eutil::Init();

    eutil::WriteFile(path, "Hello, World!");
    auto data = eutil::ReadFile(path, true);
    compare("Hello, World!", std::string(reinterpret_cast<char*>(data.get())), "ReadFile failed");
    auto result = eutil::RemoveFile(path);
    compare(true, result, "RemoveFile failed");

    std::shared_ptr<uint8_t[]> buffer(new uint8_t[13]{ 'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!' });
    eutil::WriteFile(path, buffer, 13);
    data = eutil::ReadFile(path, true);
    compare("Hello, World!", std::string(reinterpret_cast<char*>(data.get())), "ReadFile failed: buffer");
    result = eutil::RemoveFile(path);
    compare(true, result, "RemoveFile failed: buffer");

    std::ofstream file(path);
    eutil::WriteFile(file, "Hello, World!");
    file.close();
    data = eutil::ReadFile(path, true);
    compare("Hello, World!", std::string(reinterpret_cast<char*>(data.get())), "ReadFile failed: ofstream");
    result = eutil::RemoveFile(path);
    compare(true, result, "RemoveFile failed: ofstream");

    file.open(path);
    eutil::WriteFile(file, buffer, 13);
    file.close();
    data = eutil::ReadFile(path, true);
    compare("Hello, World!", std::string(reinterpret_cast<char*>(data.get())), "ReadFile failed: ofstream buffer");
    result = eutil::RemoveFile(path);
    compare(true, result, "RemoveFile failed: ofstream buffer");


    return ::FAILED;
}