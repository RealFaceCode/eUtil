#pragma once

#include <chrono>
#include <ctime>

#include "eutil/defines.hpp"

namespace eutil
{
    EUTIL_API std::string getCurrentTime(std::string_view format = "%Y-%m-%d %H:%M:%S");
    EUTIL_API auto getCurrentTimeDate(std::string_view timeFormat = "%H:%M:%S", std::string_view dateFormat = "%Y-%m-%d");
}