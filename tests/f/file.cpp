#include "file.hpp"
#include "../test_util.hpp"
#include "eutil.hpp"

int main()
{
    eutil::Init();

    eutil::File write("test.txt", eutil::FileOpenMode::Write);
    write.open();
    write.push_back(std::string("Hello, World!"), true);
    write.write();
    write.close();

    eutil::File file("test.txt", eutil::FileOpenMode::Read);
    file.open();
    std::cout << "File size: " << file.size() << std::endl;
    file.read();

    auto first = file.toString();

    ::compare("Hello, World!", first, "File read failed.");
    ::compare(strlen("Hello, World!"), file.size(), "File size dont match.");

    file.close();

    return ::FAILED;
}