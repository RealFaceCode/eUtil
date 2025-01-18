#pragma once

#include <unordered_map>
#include <functional>
#include <string>

#include "eutil/defines.hpp"

namespace util
{
    EUTIL_API std::unordered_map<std::string, std::function<bool(void*, std::string_view)>>& GetSaveRules();
    EUTIL_API std::unordered_map<std::string, std::function<bool(void*, std::string_view)>>& GetLoadRules();

    template<typename T>
    void AddSaveRule(bool (*func)(void*, std::string_view))
    {
        std::string key = typeid(T).name();
        GetSaveRules()[key] = func;
    } 

    template<typename T>
    void AddLoadRule(bool (*func)(void*, std::string_view))
    {
        std::string key = typeid(T).name();
        GetLoadRules()[key] = func;
    }

    template<typename T>
    bool UseSaveRule(void* data, std::string_view path)
    {
        std::string key = typeid(T).name();
        auto SaveRules = GetSaveRules();
        auto it = SaveRules.find(key);
        if(it == SaveRules.end())
            return false;

        return it->second(data, path);
    }

    template<typename T>
    bool UseLoadRule(void* data, std::string_view path)
    {
        std::string key = typeid(T).name();
        auto LoadRules = GetLoadRules();
        auto it = LoadRules.find(key);
        if(it == LoadRules.end())
            return false;

        return it->second(data, path);
    }
}