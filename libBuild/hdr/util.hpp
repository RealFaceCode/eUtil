#pragma once

#include <chrono>
#include <ctime>

namespace eutil
{
    std::string getCurrentTime(std::string_view format = "%Y-%m-%d %H:%M:%S");
    auto getCurrentTimeDate(std::string_view timeFormat = "%H:%M:%S", std::string_view dateFormat = "%Y-%m-%d");
}