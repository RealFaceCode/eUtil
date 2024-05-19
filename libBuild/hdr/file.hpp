#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <memory>
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
    EUTIL_API bool IsSymlink(const std::filesystem::path& path);
    EUTIL_API bool IsRelative(const std::filesystem::path& path);
    EUTIL_API bool IsAbsolute(const std::filesystem::path& path);
    EUTIL_API size_t FileSize(const std::filesystem::path& path);
    EUTIL_API std::string FileExtension(const std::filesystem::path& path);
    EUTIL_API std::string FileName(const std::filesystem::path& path);
    EUTIL_API std::string FileNameWithoutExtension(const std::filesystem::path& path);
    EUTIL_API std::string FileDirectory(const std::filesystem::path& path);
    EUTIL_API std::string FileDirectoryRoot(const std::filesystem::path& path);
    EUTIL_API std::string FileCanonical(const std::filesystem::path& path);
    EUTIL_API std::string FileRelative(const std::filesystem::path& path, const std::filesystem::path& base);
    EUTIL_API std::string FileAbsolute(const std::filesystem::path& path);
    EUTIL_API std::string FileNormalize(const std::filesystem::path& path);
    EUTIL_API std::shared_ptr<uint8_t[]> ReadFile(const std::filesystem::path& path, bool putZeroAtEnd = false);
    EUTIL_API bool WriteFile(const std::filesystem::path& path, std::shared_ptr<uint8_t[]> data, size_t size);
    EUTIL_API bool AppendFile(const std::filesystem::path& path, std::shared_ptr<uint8_t[]> data, size_t size);
    EUTIL_API bool WriteFile(const std::filesystem::path& path, std::string_view data);
    EUTIL_API bool AppendFile(const std::filesystem::path& path, std::string_view data);
    EUTIL_API std::shared_ptr<uint8_t[]> ReadFile(std::ifstream& file, size_t size);
    EUTIL_API bool WriteFile(std::ofstream& file, std::shared_ptr<uint8_t[]> data, size_t size);
    EUTIL_API bool AppendFile(std::ofstream& file, std::shared_ptr<uint8_t[]> data, size_t size);
    EUTIL_API bool WriteFile(std::ofstream& file, std::string_view data);
    EUTIL_API bool AppendFile(std::ofstream& file, std::string_view data);

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
        void push_back(const T& value, bool only_str = false)
        {
            m_buffer.push_back(value, only_str);
            m_size = m_buffer.size();
        }

        template<typename T>
        T getNextElement(bool only_str = false)
        {
            return m_buffer.getNextElement<T>(only_str);
        }
        
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