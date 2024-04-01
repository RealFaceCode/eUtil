#pragma once

#include <chrono>
#include <ctime>

namespace eutil
{
    std::string getCurrentTime(std::string_view format = "%Y-%m-%d %H:%M:%S");
}