#pragma once

namespace util
{
    struct FilePermission
    {
    public:
        bool read;
        bool write;
        bool execute;
    };
}