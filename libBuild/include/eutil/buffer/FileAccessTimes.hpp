#pragma once

namespace util
{
    struct FileAccessTimes
    {
        unsigned long long creation;
        unsigned long long lastAccess;
        unsigned long long lastWrite;
    };
}