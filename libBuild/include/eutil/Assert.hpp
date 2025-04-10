#pragma once

#include "defines.hpp"
#include <string>

namespace util
{
    EUTIL_API void AssertFailed(const std::string& condition, const std::string& message, const std::string& file, int line);

    #if defined(__cpp_exceptions) && !defined(NDEBUG)
        #define ASSERT(condition) \
            do { \
                if (!(condition)) { \
                    util::AssertFailed(#condition, "", __FILE__, __LINE__); \
                } \
            } while (false)
        #define ASSERT_MSG(condition, message) \
            do { \
                if (!(condition)) { \
                    util::AssertFailed(#condition, message, __FILE__, __LINE__); \
                } \
            } while (false)
    #else
            #define ASSERT(condition) do { (void)(sizeof(condition)) } while (false)
            #define ASSERT_MSG(condition, message) do { (void)(sizeof(condition)) } while (false)
    #endif
}

