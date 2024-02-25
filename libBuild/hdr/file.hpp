#pragma once

#include <filesystem>
#include "binaryarraybuffer.hpp"

namespace eutil
{
    struct File
    {
    public:
        File();
        explicit File(const std::filesystem::path& path);
        File(const File& other);
        File(File&& other) noexcept;
        ~File();

        File& operator=(const File& other);
        File& operator=(File&& other) noexcept;

        void open();
        void close();

        bool isOpen() const;

        BinaryArrayBuffer& data();
        const BinaryArrayBuffer& data() const;
        std::filesystem::path path() const;
        size_t size() const;

        void read();
        void write();

    private:
        std::filesystem::path m_path    = "";
        BinaryArrayBuffer m_buffer;
        bool m_isOpen                   = false;
        size_t m_size                   = 0;
        FILE* m_file                    = nullptr;
    };
}