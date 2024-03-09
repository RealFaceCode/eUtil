#include "timer.hpp"

namespace eutil
{
    Timer::Timer() = default;

    Timer::~Timer() = default;

    void Timer::start()
    {
        m_running = true;
        m_start = std::chrono::high_resolution_clock::now();
    }

    void Timer::stop()
    {
        m_running = false;
        m_elapsed = std::chrono::high_resolution_clock::now();
    }

    void Timer::reset()
    {
        m_running = false;
        m_start = std::chrono::high_resolution_clock::now();
        m_elapsed = std::chrono::high_resolution_clock::now();
    }

    double Timer::elapsed() const
    {
        if (m_running)
            return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - m_start).count();
        else
            return std::chrono::duration_cast<std::chrono::duration<double>>(m_elapsed - m_start).count();
    }
}