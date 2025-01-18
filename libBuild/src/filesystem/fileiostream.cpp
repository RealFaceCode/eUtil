#include "eutil/filesystem/fileiostream.hpp"
#include "eutil/filesystem/fileiocore.hpp"
#include "eutil/buffer/array.hpp"

#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <string>

namespace util
{
    static std::unordered_map<std::string, FILE*> open_files = {};

    EUTIL_API FILE** getFile(const char* path, const char* mode)
    {
        auto it = open_files.find(path);
        if(it != open_files.end())
            return &it->second;

        FILE* file = nullptr;
        open_file(path, mode, &file);
        if(file != nullptr)
            open_files.emplace(path, file);
        return open_files.find(path) != open_files.end() ? &open_files[path] : nullptr; 
    }

    EUTIL_API void updateMap()
    {
        for(auto it = open_files.begin(); it != open_files.end();)
        {
            if(!IsFileOpen(it->second))
                it = open_files.erase(it);
            else
                ++it;
        }
    }

    EUTIL_API uint8_t* ReadDataFromFile(const char* path, size_t& size, bool nullterminate, bool close)
    {
        FILE** file = getFile(path, "rb");
        if(file == nullptr)
            return nullptr;

        bool succes = GetFileSize(path, size);
        if(!succes)
            return nullptr;

        uint8_t* data = nullptr;
        void* data_ptr = nullptr;
        ReadDataFromFileRaw(file, path, &data_ptr, size, nullterminate ? size + 1 : size, close);
        data = static_cast<uint8_t*>(data_ptr);

        if(nullterminate)
            data[size] = '\0';

        updateMap();

        return data;
    }

    EUTIL_API Array ReadDataFromFile(const char* path, bool close)
    {
        size_t size = 0;
        uint8_t* data = ReadDataFromFile(path, size, false, close);
        Array array = CreateArray(size);
        WriteToArray(array, data, size);
        free(data);
        return array;
    }

    EUTIL_API bool WriteDataToFile(const char* path, const uint8_t* data, size_t size, bool close)
    {
        FILE** file = getFile(path, "wb");
        if(file == nullptr)
            return false;
        
        bool success = WriteDataToFileRaw(file, path, data, size, close);

        updateMap();

        return success;
    }

    EUTIL_API bool AppendDataToFile(const char* path, const uint8_t* data, size_t size, bool close)
    {
        FILE** file = getFile(path, "ab");
        if(file == nullptr)
            return false;
        
        bool success = AppendDataToFileRaw(file, path, data, size, close);

        updateMap();

        return success;
    }

    EUTIL_API uint8_t* ReadDataFromFile(const std::filesystem::path& path, size_t& size, bool nullterminate, bool close)
    {
        return ReadDataFromFile(path.string().c_str(), size, nullterminate, close);
    }

    EUTIL_API Array ReadDataFromFile(const std::filesystem::path& path, bool close)
    {
        return ReadDataFromFile(path.string().c_str(), close);
    }

    EUTIL_API bool WriteDataToFile(const std::filesystem::path& path, const uint8_t* data, size_t size, bool close)
    {
        return WriteDataToFile(path.string().c_str(), data, size, close);
    }

    EUTIL_API bool AppendDataToFile(const std::filesystem::path& path, const uint8_t* data, size_t size, bool close)
    {
        return AppendDataToFile(path.string().c_str(), data, size, close);
    }
}