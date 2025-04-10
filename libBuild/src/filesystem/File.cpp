#include "eutil/filesystem/File.hpp"
#include "eutil/Assert.hpp"
#include "eutil/filesystem/FileIOGeneric.hpp"

namespace util
{
    File::File(const std::filesystem::path& path, FileMode mode)
    : path(path), mode(mode)
    {
        if (!path.empty() && FileExist(path))
        {
            accessTimes = GetFileTime(path).value_or(FileAccessTimes{});
            permission = GetFilePermission(path).value_or(FilePermission{});
            fileContent.resize(GetFileSize(path));
        }
        else if(!FileExist(path))
        {
            if(!CreateNewFile(path))
            {
                auto errorMsg = std::format("Failed to create new file: {}", path.string());
                ASSERT_MSG(false, errorMsg);
            }

            accessTimes = GetFileTime(path).value_or(FileAccessTimes{});
            permission = GetFilePermission(path).value_or(FilePermission{});
            fileContent.resize(GetFileSize(path));
        }
    }

    File::File(File&& other) noexcept
    : mode(other.mode), path(std::move(other.path)), accessTimes(std::move(other.accessTimes)), permission(std::move(other.permission)), fileContent(std::move(other.fileContent))
    {
        other.mode = FileMode::Read;
        other.path.clear();
        other.fileContent.clear();
    }

    File& File::operator=(File&& other) noexcept
    {
        if (this != &other)
        {
            mode = other.mode;
            path = std::move(other.path);
            accessTimes = std::move(other.accessTimes);
            permission = std::move(other.permission);
            fileContent = std::move(other.fileContent);

            other.mode = FileMode::Read;
            other.path.clear();
            other.fileContent.clear();
        }
        return *this;
    }

    bool File::open()
    {
        if (fileHandle.is_open())
            return true;
        
        if (path.empty())
            return false;

        std::ios_base::openmode openMode = std::ios_base::binary;
        switch(mode)
        {
            case FileMode::Read:
                openMode |= std::ios_base::in;
                break;
            case FileMode::Write:
                openMode |= std::ios_base::out | std::ios_base::trunc;
                break;
            case FileMode::Append:
                openMode |= std::ios_base::out | std::ios_base::app;
                break;
            case FileMode::ReadWrite:
                openMode |= std::ios_base::in | std::ios_base::out;
                break;
        }

        fileHandle.open(path, openMode);

        if (!fileHandle.is_open())
            return false;

        if (mode == FileMode::Read || mode == FileMode::ReadWrite)
        {
            fileHandle.seekg(0, std::ios_base::end);
            fileContent.resize(fileHandle.tellg());
            fileHandle.seekg(0, std::ios_base::beg);
        }

        if (mode == FileMode::Write || mode == FileMode::ReadWrite)
            fileHandle.seekp(0, std::ios_base::end);

        return true;
    }

    bool File::open(const std::filesystem::path& path, FileMode mode)
    {
        this->path = path;
        this->mode = mode;
        return open();
    }

    void File::close()
    {
        if (fileHandle.is_open())
            fileHandle.close();
    }

    bool File::isOpen() const
    {
        return fileHandle.is_open();
    }

    bool File::isEmpty() const
    {
        return fileContent.isEmpty();
    }

    void File::write(const void* data, size_t size)
    {
        newFileContent.write(data, size);
    }

    void File::write(const uint8_t* data, size_t size)
    {
        newFileContent.write(data, size);
    }

    void File::read(void* data, size_t size)
    {
        fileContent.read(data, size);
    }

    void File::read(uint8_t* data, size_t size)
    {
        fileContent.read(data, size);
    }

    bool File::save()
    {
        if (!fileHandle.is_open())
            return false;

        fileHandle.write(reinterpret_cast<const char*>(newFileContent.data()), newFileContent.size());
        if (!fileHandle.good())
            return false;

        fileHandle.flush();
        fileContent.write(newFileContent.data(), newFileContent.size());
        newFileContent.clear();

        return true;
    }

    bool File::load()
    {
        if (!fileHandle.is_open())
            return false;

        auto fileSize = GetFileSize(path);
        if (fileSize == 0)
            return false;

        if(!fileContent.readFrom(fileHandle, fileSize))
            return false;

        return true;
    }

    void File::clear()
    {
        fileContent.clear();
        newFileContent.clear();
    }

    void File::zero()
    {
        fileContent.zero();
        newFileContent.zero();
    }

    void File::reset()
    {
        fileContent.reset();
        newFileContent.reset();
    }

    void File::setWriteOffset(size_t offset)
    {
        newFileContent.setWriteOffset(offset);
    }

    void File::setReadOffset(size_t offset)
    {
        fileContent.setReadOffset(offset);
    }

    void File::setPath(const std::filesystem::path& path)
    {
        this->path = path;
    }
}