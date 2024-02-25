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
    };
}