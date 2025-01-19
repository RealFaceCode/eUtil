#pragma once

#include "eutil/defines.hpp"

#include <filesystem>
#include <optional>

namespace util
{
    struct ImageData;
    struct Array;

    EUTIL_API std::optional<Array> LoadFile(const std::filesystem::path& path);
    EUTIL_API std::optional<ImageData> LoadImage(const std::filesystem::path& path, bool flip = false);
}