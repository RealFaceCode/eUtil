#include "eutil/filesystem/FileLoad.hpp"
#include "eutil/filesystem/FileIOGeneric.hpp"
#include "eutil/buffer/imagedata.hpp"
#include "eutil/buffer/Array.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fstream>

namespace util
{
    std::optional<Array> LoadFile(const std::filesystem::path& path)
    {
        if(!FileExist(path))
            return std::nullopt;

        Array array;
        size_t size = GetFileSize(path);
        array.resize(size);

        std::fstream file(path, std::ios::binary); 
        file.open(path, std::ios::in | std::ios::binary);
        
        if(file.is_open())
        {
            array.readFrom(file, size);
            file.close();
        }
        else
            return std::nullopt;

        return array;
    }

    std::optional<ImageData> LoadImage(const std::filesystem::path& path, bool flip)
    {
        if(!FileExist(path))
            return std::nullopt;

        ImageData img;

        int width;
        int height;
        int channels;
        ::stbi_set_flip_vertically_on_load(flip);
        unsigned char* data = ::stbi_load(path.string().c_str(), &width, &height, &channels, 0);
        if(data)
        {
            img.set(width, height, channels, data);
            ::stbi_image_free(data);
        }
        else 
            return std::nullopt;

        return img;
    }
}