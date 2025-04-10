#pragma once

#include <filesystem>
#include <fstream>

#include "eutil/defines.hpp"
#include "eutil/conceps.hpp"
#include "eutil/buffer/FileAccessTimes.hpp"
#include "eutil/buffer/FilePermission.hpp"
#include "eutil/buffer/Array.hpp"

namespace util
{

    enum class FileMode
    {
        Read,
        Write,
        Append,
        ReadWrite
    };

    struct EUTIL_API File
    {
    public:
        File() = default;
        File(const std::filesystem::path& path, FileMode mode = FileMode::Read);
        ~File() = default;
        File(const File&) = delete;
        
        File& operator=(const File&) = delete;
        File(File&& other) noexcept;
        File& operator=(File&& other) noexcept;

        bool open();
        bool open(const std::filesystem::path& path, FileMode mode = FileMode::Read);
        void close();
        bool isOpen() const;
        bool isEmpty() const;

        void write(const void* data, size_t size);
        void write(const uint8_t* data, size_t size);
        void read(void* data, size_t size);
        void read(uint8_t* data, size_t size);

        template<typename T>
        void write(const T& data)
        {
            newFileContent.write<T>(data);
        }

        template<typename T>
        T read()
        {
            return fileContent.read<T>();
        }

        bool save();
        bool load();
        
        void clear();
        void zero();
        void reset();
        void setWriteOffset(size_t offset);
        void setReadOffset(size_t offset);
        void setPath(const std::filesystem::path& path);
    
    private:
        std::fstream fileHandle;
        FileMode mode;
        std::filesystem::path path;
        FileAccessTimes accessTimes;
        FilePermission permission;
        Array fileContent;
        Array newFileContent;
    };
}