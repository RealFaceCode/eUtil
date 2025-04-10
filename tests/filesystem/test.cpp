#include "../test_util.hpp"

#include "eutil/filesystem/File.hpp"
#include "eutil/filesystem/FileIOGeneric.hpp"

int main()
{
    util::File file("example.txt", util::FileMode::Write);

    if (!file.open())
    {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    file.write("Hello, World!");
    if (!file.save())
    {
        std::cerr << "Failed to save file." << std::endl;
        return 1;
    }

    file.save();
    file.close();

    util::File readFile("example.txt", util::FileMode::Read);
    if (!readFile.open())
    {
        std::cerr << "Failed to open file for reading." << std::endl;
        return 1;
    }

    readFile.load();
    std::string content = readFile.read<std::string>();
    readFile.close();

    ::compare("Hello, World!", content, "Failed to read file content");

    util::RemoveFile("example.txt");

    auto exist = util::FileExist("example.txt");
    ::compare(false, exist, "File should not exist after deletion");

    return ::FAILED;
}