#include "fios.hpp"
#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <string>
#include "fioc.hpp"
#include "ahc.hpp"

namespace eutil::fios
{
    static std::unordered_map<std::string, FILE*> open_files = {};

    EUTIL_API FILE** getFile(const char* path, const char* mode)
    {
        auto it = open_files.find(path);
        if(it != open_files.end())
            return &it->second;

        FILE* file = nullptr;
        fioc::open_file(path, mode, &file);
        if(file != nullptr)
            open_files.emplace(path, file);
        return open_files.find(path) != open_files.end() ? &open_files[path] : nullptr; 
    }

    EUTIL_API void updateMap()
    {
        for(auto it = open_files.begin(); it != open_files.end();)
        {
            auto f = it->second;
            if(!fioc::IsFileOpen(it->second))
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

        bool succes = fioc::GetFileSize(path, size);

        uint8_t* data = nullptr;
        fioc::ReadDataFromFileRaw(file, path, reinterpret_cast<void**>(&data) ,size, nullterminate ? size + 1 : size, close);

        if(nullterminate)
            data[size] = '\0';

        updateMap();

        return data;
    }

    EUTIL_API ahc::Array ReadDataFromFile(const char* path, bool close)
    {
        size_t size = 0;
        uint8_t* data = ReadDataFromFile(path, size, false, close);
        ahc::Array array = ahc::CreateArray(size);
        ahc::WriteToArray(array, data, size);
        free(data);
        return array;
    }

    EUTIL_API bool WriteDataToFile(const char* path, const uint8_t* data, size_t size, bool close)
    {
        FILE** file = getFile(path, "wb");
        if(file == nullptr)
            return false;
        
        bool success = fioc::WriteDataToFileRaw(file, path, data, size, close);

        updateMap();

        return success;
    }

    EUTIL_API bool AppendDataToFile(const char* path, const uint8_t* data, size_t size, bool close)
    {
        FILE** file = getFile(path, "ab");
        if(file == nullptr)
            return false;
        
        bool success = fioc::AppendDataToFileRaw(file, path, data, size, close);

        updateMap();

        return success;
    }
}