#pragma once

#include "eutil/defines.hpp"

#include <ctime>
#include <string>
#include <unordered_map>
#include <filesystem>

namespace util
{
    struct FileWatch
    {
        time_t mCheckTime;
        bool mChanged;
    };

    struct Array;

    struct EUTIL_API HotLoader
    {
        static HotLoader& GetInstance();
        
        HotLoader() = default;
        ~HotLoader();

        void addFile(const std::filesystem::path& filePath);
        void removeFile(const std::filesystem::path& filePath);
        bool isModified(const std::filesystem::path& filePath);
        Array getFile(const std::filesystem::path& filePath);
        std::string getFileAsString(const std::filesystem::path& filePath);
        void checkFiles();
        void clear();

        size_t size() const;
        void print() const;
    private:
        std::unordered_map<std::filesystem::path, FileWatch> mFiles;
    };
} // namespace util
