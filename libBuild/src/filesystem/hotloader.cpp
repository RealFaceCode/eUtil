#include "eutil/filesystem/hotloader.hpp"
#include "eutil/filesystem/fioc.hpp"
#include "eutil/filesystem/fios.hpp"
#include "eutil/buffer/ahc.hpp"

#include <print>

namespace util
{
    HotLoader& HotLoader::GetInstance()
    {
        static HotLoader instance;
        return instance;
    }

    HotLoader::~HotLoader()
    {
        clear();
    }

    void HotLoader::addFile(const std::filesystem::path& filePath)
    {
        if(!FileExists(filePath))
            return;

        auto it = mFiles.find(filePath);
        if (it != mFiles.end())
            return;

        mFiles[filePath] = { std::filesystem::last_write_time(filePath).time_since_epoch().count(), false };
    }

    void HotLoader::removeFile(const std::filesystem::path& filePath)
    {
        auto it = mFiles.find(filePath);
        if (it == mFiles.end())
            return;

        mFiles.erase(it);
    }

    bool HotLoader::isModified(const std::filesystem::path& filePath)
    {
        auto it = mFiles.find(filePath);
        if(it == mFiles.end())
            return false;

        return it->second.mChanged;
    }

    Array HotLoader::getFile(const std::filesystem::path& filePath)
    {
        if (!FileExists(filePath))
            return Array();

        return ReadDataFromFile(filePath, false);
    }

    std::string HotLoader::getFileAsString(const std::filesystem::path& filePath)
    {
        if (!FileExists(filePath))
            return "";

        std::string data;
        size_t size;
        uint8_t* buffer = ReadDataFromFile(filePath, size, false);
        if (buffer)
        {
            data = std::string(reinterpret_cast<char*>(buffer), size);
            delete[] buffer;
        }

        return data;
    }

    void HotLoader::checkFiles()
    {
        for (auto& [path, file] : mFiles)
        {
            time_t lastWriteTime = std::filesystem::last_write_time(path).time_since_epoch().count();
            if (lastWriteTime != file.mCheckTime)
            {
                file.mCheckTime = lastWriteTime;
                file.mChanged = true;
            }
            else
                file.mChanged = false;
        }
    }

    void HotLoader::clear()
    {
        for (auto it = mFiles.begin(); it != mFiles.end();)
        {
            if (it->second.mChanged)
            {
                it = mFiles.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    size_t HotLoader::size() const
    {
        return mFiles.size();
    }

    void HotLoader::print() const
    {
        for (auto& [path, file] : mFiles)
        {
            std::print("File Watched: {}\n", path.string());
        }
    }
}