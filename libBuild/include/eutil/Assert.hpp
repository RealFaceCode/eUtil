#pragma once

#include "defines.hpp"
#include <string>
#include <cstdlib>

namespace util
{
    inline void AssertFailed(const std::string& condition, const std::string& message, const std::string& file, int line)
    {
        if(!message.empty())
            std::print("Assertion failed: {}\nFile: {}\nLine: {}\nMessage: {}\n", condition, file, line, message);
        else
            std::print("Assertion failed: {}\nFile: {}\nLine: {}\n", condition, file, line);

        std::abort();
    }

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

