#include "../test_util.hpp"
#include "eutil/slr.hpp"
#include "eutil/fios.hpp"

int main()
{
    // Test code
    eutil::slr::AddSaveRule<std::string>([](void* data, std::string_view path) -> bool
    {
        std::string* str = (std::string*)data;
        eutil::fios::WriteDataToFile(path.data(), reinterpret_cast<const uint8_t*>(str->data()), str->size());
        return true;
    });

    eutil::slr::AddLoadRule<std::string>([](void* data, std::string_view path) -> bool
    {
        size_t size = 0;
        auto buffer = eutil::fios::ReadDataFromFile(path.data(), size, true);
        std::string* str = (std::string*)data;
        str->assign((char*)(buffer), size);
        free(buffer);
        return true;
    });

    std::string saveData = "Hello, World! Wonderfull day!";
    eutil::slr::UseSaveRule<std::string>(&saveData, "test.txt");

    std::string loadData;
    eutil::slr::UseLoadRule<std::string>(&loadData, "test.txt");

    compare(saveData, loadData, "slr Save/Load test");

    return ::FAILED;
}