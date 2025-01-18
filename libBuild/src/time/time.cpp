#include "eutil/time/time.hpp"

namespace util
{
    EUTIL_API std::string getCurrentTime(std::string_view format)
    {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::stringstream timeStringStream;
        std::tm localTime;
#ifdef _WIN32
        ::localtime_s(&localTime, &now_c);
#elif __linux__
        ::localtime_r(&now_c, &localTime);
#endif
        timeStringStream << std::put_time(&localTime, format.data());
        return timeStringStream.str();
    }

    EUTIL_API auto getCurrentTimeDate(std::string_view timeFormat, std::string_view dateFormat)
    {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        
        std::tm localTime;
#ifdef _WIN32
        ::localtime_s(&localTime, &now_c);
#elif __linux__
        ::localtime_r(&now_c, &localTime);
#endif
        
        std::stringstream timeStringStream;
        timeStringStream << std::put_time(&localTime, timeFormat.data());
        
        std::stringstream dateStringStream;
        dateStringStream << std::put_time(&localTime, dateFormat.data());
        
        return std::make_pair(timeStringStream.str(), dateStringStream.str());
    }
}