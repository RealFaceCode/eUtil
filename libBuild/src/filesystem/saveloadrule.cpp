#include "eutil/filesystem/saveloadrule.hpp"

namespace util
{
    std::unordered_map<std::string, std::function<bool(void*, std::string_view)>> SaveRules;
    std::unordered_map<std::string, std::function<bool(void*, std::string_view)>> LoadRules;

    EUTIL_API std::unordered_map<std::string, std::function<bool(void*, std::string_view)>>& GetSaveRules()
    {
        return SaveRules;
    }

    EUTIL_API std::unordered_map<std::string, std::function<bool(void*, std::string_view)>>& GetLoadRules()
    {
        return LoadRules;
    }
}