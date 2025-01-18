#include "../test_util.hpp"
#include "eutil/filesystem/slr.hpp"
#include "eutil/filesystem/fios.hpp"

int main()
{
    // Test code
    util::AddSaveRule<std::string>([](void* data, std::string_view path) -> bool
    {
        std::string* str = (std::string*)data;
        util::WriteDataToFile(path.data(), reinterpret_cast<const uint8_t*>(str->data()), str->size());
        return true;
    });

    util::AddLoadRule<std::string>([](void* data, std::string_view path) -> bool
    {
        size_t size = 0;
        auto buffer = util::ReadDataFromFile(path.data(), size, true);
        std::string* str = (std::string*)data;
        str->assign((char*)(buffer), size);
        free(buffer);
        return true;
    });

    std::string saveData = "Hello, World! Wonderfull day!";
    util::UseSaveRule<std::string>(&saveData, "test.txt");

    std::string loadData;
    util::UseLoadRule<std::string>(&loadData, "test.txt");

    compare(saveData, loadData, "slr Save/Load test");

    return ::FAILED;
}