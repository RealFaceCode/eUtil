#include "eutil/buffer/imagedata.hpp"

namespace util
{
    ImageData::ImageData()
        : imgWidth(0), imgHeight(0), imgChannels(0), imgData({})
    {}

    ImageData::ImageData(unsigned int width, unsigned int height, unsigned int channels, const unsigned char* data)
        : imgWidth(width), imgHeight(height), imgChannels(channels), imgData({})
    {
        CreateArray(width * height * channels);
        WriteToArray(imgData, data, width * height * channels);
    }

    ImageData::ImageData(ImageData&& other) noexcept
        : imgWidth(other.imgWidth), imgHeight(other.imgHeight), imgChannels(other.imgChannels), imgData(other.imgData)
    {
        other.imgWidth = 0;
        other.imgHeight = 0;
        other.imgChannels = 0;
        other.imgData = {};
    }

    ImageData::~ImageData()
    {
        FreeArray(imgData);
    }

    void ImageData::set(unsigned int width, unsigned int height, unsigned int channels, const unsigned char* data)
    {
        imgWidth = width;
        imgHeight = height;
        imgChannels = channels;
        WriteToArray(imgData, data, width * height * channels);
    }

    unsigned int ImageData::width() const
    {
        return imgWidth;
    }

    unsigned int ImageData::height() const
    {
        return imgHeight;
    }

    unsigned int ImageData::channels() const
    {
        return imgChannels;
    }

    const unsigned char* ImageData::data() const
    {
        return imgData.data;
    }

    ImageData& ImageData::operator=(const ImageData& other)
    {
        if(this != &other)
        {
            imgWidth = other.imgWidth;
            imgHeight = other.imgHeight;
            imgChannels = other.imgChannels;
            imgData = other.imgData;
        }
        return *this;
    }

    ImageData& ImageData::operator=(ImageData&& other) noexcept
    {
        if(this != &other)
        {
            imgWidth = other.imgWidth;
            imgHeight = other.imgHeight;
            imgChannels = other.imgChannels;
            imgData = other.imgData;

            other.imgWidth = 0;
            other.imgHeight = 0;
            other.imgChannels = 0;
            other.imgData = {};
        }
        return *this;
    }
}