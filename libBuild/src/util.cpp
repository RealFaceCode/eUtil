#include "util.hpp"

namespace eutil
{
    std::string getCurrentTime(std::string_view format)
    {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        
        std::tm const* localTime = std::localtime(&now_c);
        std::stringstream timeStringStream;
        timeStringStream << std::put_time(localTime, format.data());
        
        return timeStringStream.str();
    }
}