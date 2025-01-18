#pragma once

#include "eutil/defines.hpp"
#include "eutil/buffer/array.hpp"

namespace util
{

    struct EUTIL_API ImageData
    {
        ImageData();
        ImageData(unsigned int width, unsigned int height, unsigned int channels, const unsigned char* data);
        ImageData(const ImageData& other) = default;
        ImageData(ImageData&& other) noexcept;
        ~ImageData();

        void set(unsigned int width, unsigned int height, unsigned int channels, const unsigned char* data);

        unsigned int width() const;
        unsigned int height() const;
        unsigned int channels() const;
        const unsigned char* data() const;

        ImageData& operator=(const ImageData& other);
        ImageData& operator=(ImageData&& other) noexcept;

    private:
        unsigned int imgWidth;
        unsigned int imgHeight;
        unsigned int imgChannels;
        Array imgData;
    };
}