#pragma once

#include <chrono>

#include "defines.hpp"

namespace eutil
{
    class EUTIL_API SignalTimer
    {
    public:
        SignalTimer() = default;
        explicit SignalTimer(double seconds, double secondsActive = 0);
        ~SignalTimer() = default;

        void setSeconds(double seconds);

        void start();
        void stop();
        void reset();

        double elapsed() const;
        bool check();

    private:
        bool m_running = false;
        double m_seconds = 0;
        double m_secondsActive = 0;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_elapsed;
    };
}