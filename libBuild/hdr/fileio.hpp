#pragma once

#include <filesystem>
#include <fstream>

#include "defines.hpp"
#include "binarybuffer.hpp"

namespace eutil
{

    enum class FileMode
    {
        Read,
        Write,
        Append
    };

    struct EUTIL_API File
    {
        File(const std::filesystem::path& path, FileMode mode);
        ~File();

        void open();
        void close();

        void write();
        void read();

        template<typename T> void push(const T& data) { m_buffer.push(data); }
        template<typename T> T get() { return m_buffer.get<T>(); }

    private:
        BinaryBuffer m_buffer;
        std::filesystem::path m_path;
        FileMode m_mode;
        std::fstream m_ifs;
    };
}