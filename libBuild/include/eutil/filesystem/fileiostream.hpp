#pragma once

#include <cstdint>
#include <filesystem>

#include "eutil/defines.hpp"

namespace util
{
    struct Array;

    EUTIL_API uint8_t* ReadDataFromFile(const char* path, size_t& size, bool nullterminate, bool close = true);
    EUTIL_API Array ReadDataFromFile(const char* path, bool close = true);
    EUTIL_API bool WriteDataToFile(const char* path, const uint8_t* data, size_t size, bool close = true);
    EUTIL_API bool AppendDataToFile(const char* path, const uint8_t* data, size_t size, bool close = true);
    EUTIL_API uint8_t* ReadDataFromFile(const std::filesystem::path& path, size_t& size, bool nullterminate, bool close = true);
    EUTIL_API Array ReadDataFromFile(const std::filesystem::path& path, bool close = true);
    EUTIL_API bool WriteDataToFile(const std::filesystem::path& path, const uint8_t* data, size_t size, bool close = true);
    EUTIL_API bool AppendDataToFile(const std::filesystem::path& path, const uint8_t* data, size_t size, bool close = true);
}