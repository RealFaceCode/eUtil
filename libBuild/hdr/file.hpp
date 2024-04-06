#pragma once

#include <filesystem>
#include "binaryarraybuffer.hpp"

namespace eutil
{
    EUTIL_API bool FileExists(const std::filesystem::path& path);
    EUTIL_API bool IsDirectory(const std::filesystem::path& path);
    EUTIL_API bool IsFile(const std::filesystem::path& path);
    EUTIL_API bool CreateFile(const std::filesystem::path& path);
    EUTIL_API bool CreateDirectory(const std::filesystem::path& path);
    EUTIL_API bool RemoveFile(const std::filesystem::path& path);
    EUTIL_API bool RemoveDirectory(const std::filesystem::path& path);
    EUTIL_API bool RenameFile(const std::filesystem::path& old_path, std::string_view new_name);
    EUTIL_API bool RenameDirectory(const std::filesystem::path& old_path, std::string_view new_name);
    EUTIL_API bool CopyFile(const std::filesystem::path& old_path, const std::filesystem::path& new_path);
    EUTIL_API bool CopyDirectory(const std::filesystem::path& old_path, const std::filesystem::path& new_path);
    EUTIL_API bool MoveFile(const std::filesystem::path& old_path, const std::filesystem::path& new_path);
    EUTIL_API bool MoveDirectory(const std::filesystem::path& old_path, const std::filesystem::path& new_path);
    EUTIL_API bool IsEmpty(const std::filesystem::path& path);
    EUTIL_API bool IsReadable(const std::filesystem::path& path);
    EUTIL_API bool IsWritable(const std::filesystem::path& path);
    EUTIL_API bool IsExecutable(const std::filesystem::path& path);
    EUTIL_API bool IsHidden(const std::filesystem::path& path);

    enum class FileOpenMode
    {
        Read,
        Write,
        Append,
        ReadWrite
    };

    struct EUTIL_API File
    {
    public:
        File();
        explicit File(const std::filesystem::path& path, FileOpenMode mode);
        ~File();

        void setPath(const std::filesystem::path& path);
        void setMode(FileOpenMode mode);

        void open();
        void close();

        bool isOpen() const;

        BinaryArrayBuffer& data();
        const BinaryArrayBuffer& data() const;
        std::filesystem::path path() const;
        size_t size() const;

        void read();
        void write();

        explicit operator bool() const;

        template<typename T>
        void push_back(const T& value, bool only_str = false);

        template<typename T>
        T getNextElement(bool only_str = false);

        std::string toString();

    private:
        std::filesystem::path m_path    = "";
        BinaryArrayBuffer m_buffer;
        bool m_isOpen                   = false;
        size_t m_size                   = 0;
        FILE* m_file                    = nullptr;
        FileOpenMode m_mode             = FileOpenMode::Read;
        std::string m_openMode          = "rb";
    };
}