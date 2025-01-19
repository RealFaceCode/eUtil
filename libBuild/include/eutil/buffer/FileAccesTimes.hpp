#pragma once

namespace util
{
    struct FileAccesTimes
    {
        unsigned long long creation;
        unsigned long long lastAccess;
        unsigned long long lastWrite;
    };
}