#pragma once

#include <iostream>
#include <string>
#include <type_traits>

static bool FAILED = false;

template<typename T, typename U>
void compare(const T& expected, const U& actual, const std::string& message = "")
{
    if constexpr (std::is_same_v<T, std::wstring> || std::is_same_v<U, std::wstring>)
    {
        if (expected != actual)
        {
            std::cerr << "Test failed: " << message << std::endl;
            std::wcerr << "Expected: " << expected << std::endl;
            std::wcerr << "Actual: " << actual << std::endl;

            ::FAILED = true;
        }
    }
    else
    {
        if (expected != actual)
        {
            std::cerr << "Test failed: " << message << std::endl;
            std::cerr << "Expected: " << expected << std::endl;
            std::cerr << "Actual: " << actual << std::endl;

            ::FAILED = true;
    }
    }
    
}