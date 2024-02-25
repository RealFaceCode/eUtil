#pragma once

#include <filesystem>
#include "binaryarraybuffer.hpp"

namespace eutil
{
    bool FileExists(const std::filesystem::path& path);
    bool IsDirectory(const std::filesystem::path& path);
    bool IsFile(const std::filesystem::path& path);
    bool CreateFile(const std::filesystem::path& path);
    bool CreateDirectory(const std::filesystem::path& path);
    bool RemoveFile(const std::filesystem::path& path);
    bool RemoveDirectory(const std::filesystem::path& path);
    bool RenameFile(const std::filesystem::path& old_path, std::string_view new_name);
    bool RenameDirectory(const std::filesystem::path& old_path, std::string_view new_name);
    bool CopyFile(const std::filesystem::path& old_path, const std::filesystem::path& new_path);
    bool CopyDirectory(const std::filesystem::path& old_path, const std::filesystem::path& new_path);
    bool MoveFile(const std::filesystem::path& old_path, const std::filesystem::path& new_path);
    bool MoveDirectory(const std::filesystem::path& old_path, const std::filesystem::path& new_path);
    bool IsEmpty(const std::filesystem::path& path);
    bool IsReadable(const std::filesystem::path& path);
    bool IsWritable(const std::filesystem::path& path);
    bool IsExecutable(const std::filesystem::path& path);
    bool IsHidden(const std::filesystem::path& path);

    enum class FileOpenMode
    {
        Read,
        Write,
        Append,
        ReadWrite
    };

    struct File
    {
    public:
        File();
        explicit File(const std::filesystem::path& path, FileOpenMode mode);
        ~File();

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