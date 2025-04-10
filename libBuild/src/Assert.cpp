#include "eutil/Assert.hpp"

#include <cstdlib>
#include <print> 

namespace util
{
    void AssertFailed(const std::string& condition, const std::string& message, const std::string& file, int line)
    {
        if(!message.empty())
            std::print("Assertion failed: {}\nFile: {}\nLine: {}\nMessage: {}\n", condition, file, line, message);
        else
            std::print("Assertion failed: {}\nFile: {}\nLine: {}\n", condition, file, line);

        std::abort();
    }
}