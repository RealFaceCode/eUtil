#include "eutil/filesystem/FileWrite.hpp"
#include "eutil/filesystem/FileIOGeneric.hpp"
#include "eutil/buffer/Array.hpp"
#include "eutil/buffer/imagedata.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <fstream>

namespace util
{
    bool WriteFile(const std::filesystem::path& path, const Array& data)
    {
        std::ofstream file(path, std::ios::binary);
        if(file.is_open())
        {
            file.write(reinterpret_cast<const char*>(data.data()), data.size());
            file.close();
            return true;
        }
        return false;
    }

    bool WriteFile(const std::filesystem::path& path, const char* data)
    {
        std::ofstream file(path);
        if(file.is_open())
        {
            file << data;
            file.close();
            return true;
        }
        return false;
    }

    bool WriteFile(const std::filesystem::path& path, const uint8_t* data)
    {
        std::ofstream file(path, std::ios::binary);
        if(file.is_open())
        {
            file.write(reinterpret_cast<const char*>(data), sizeof(data));
            file.close();
            return true;
        }
        return false;
    }

    bool WriteImagePNG(const std::filesystem::path& path, const ImageData& img)
    {
        if(!FileExist(path))
            return false;

        auto result = stbi_write_png(path.string().c_str(), img.width(), img.height(), img.channels(), img.data(), img.width() * img.channels());
        return result != 0;
    }

    bool WriteImageBMP(const std::filesystem::path& path, const ImageData& img)
    {
        if(!FileExist(path))
            return false;

        auto result = stbi_write_bmp(path.string().c_str(), img.width(), img.height(), img.channels(), img.data());
        return result != 0;
    }

    bool WriteImageTGA(const std::filesystem::path& path, const ImageData& img)
    {
        if(!FileExist(path))
            return false;

        auto result = stbi_write_tga(path.string().c_str(), img.width(), img.height(), img.channels(), img.data());
        return result != 0;
    }

    bool WriteImageJPG(const std::filesystem::path& path, const ImageData& img)
    {
        if(!FileExist(path))
            return false;

        auto result = stbi_write_jpg(path.string().c_str(), img.width(), img.height(), img.channels(), img.data(), 100);
        return result != 0;
    }

    bool WriteImageHDR(const std::filesystem::path& path, const ImageData& img)
    {
        if(!FileExist(path))
            return false;

        auto* data = new float[img.width() * img.height() * img.channels()];
        for(size_t i = 0; i < img.width() * img.height() * img.channels(); ++i)
            data[i] = static_cast<float>(img.data()[i]) / 255.0f;

        auto result = stbi_write_hdr(path.string().c_str(), img.width(), img.height(), img.channels(), data);
        delete[] data;

        return result != 0;
    }


    bool WriteImage(const std::filesystem::path& path, const ImageData& img, ImageFormat format)
    {
        switch(format)
        {
            case ImageFormat::PNG:
                return WriteImagePNG(path, img);
            case ImageFormat::BMP:
                return WriteImageBMP(path, img);
            case ImageFormat::TGA:
                return WriteImageTGA(path, img);
            case ImageFormat::JPG:
                return WriteImageJPG(path, img);
            case ImageFormat::HDR:
                return WriteImageHDR(path, img);
            default:
                return false;
        }
    }
}