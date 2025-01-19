#pragma once

#include "eutil/defines.hpp"

#include <filesystem>

namespace util
{
    struct ImageData;
    struct Array;

    enum class ImageFormat
    {
        PNG,
        BMP,
        TGA,
        JPG,
        HDR
    };

    EUTIL_API bool WriteFile(const std::filesystem::path& path, const Array& data);
    EUTIL_API bool WriteFile(const std::filesystem::path& path, const char* data);
    EUTIL_API bool WriteFile(const std::filesystem::path& path, const uint8_t* data);
    EUTIL_API bool WriteImagePNG(const std::filesystem::path& path, const ImageData& img);
    EUTIL_API bool WriteImageBMP(const std::filesystem::path& path, const ImageData& img);
    EUTIL_API bool WriteImageTGA(const std::filesystem::path& path, const ImageData& img);
    EUTIL_API bool WriteImageJPG(const std::filesystem::path& path, const ImageData& img);
    EUTIL_API bool WriteImageHDR(const std::filesystem::path& path, const ImageData& img);
    EUTIL_API bool WriteImage(const std::filesystem::path& path, const ImageData& img, ImageFormat format);

}