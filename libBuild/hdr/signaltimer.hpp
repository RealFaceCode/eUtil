#pragma once

#include <chrono>

#include "defines.hpp"

namespace eutil
{
    class EUTIL_API SignalTimer
    {
    public:
        EUTIL_API SignalTimer();
        EUTIL_API explicit SignalTimer(double seconds, double secondsActive = 0);
        EUTIL_API ~SignalTimer();

        EUTIL_API void start();
        EUTIL_API void stop();
        EUTIL_API void reset();

        EUTIL_API double elapsed() const;
        EUTIL_API bool check();

    private:
        bool m_running = false;
        double m_seconds = 0;
        double m_secondsActive = 0;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_elapsed;
    };
}