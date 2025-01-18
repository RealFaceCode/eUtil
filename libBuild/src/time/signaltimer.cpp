#include "eutil/time/signaltimer.hpp"

namespace eutil
{
    SignalTimer::SignalTimer(double seconds, double secondsActive)
        : m_seconds(seconds), m_secondsActive(secondsActive)
    {}

    void SignalTimer::setSeconds(double seconds)
    {
        m_seconds = seconds;
    }

    void SignalTimer::setSecondsActive(double secondsActive)
    {
        m_secondsActive = secondsActive;
    }

    void SignalTimer::start()
    {
        m_running = true;
        m_start = std::chrono::high_resolution_clock::now();
    }

    void SignalTimer::stop()
    {
        m_running = false;
        m_elapsed = std::chrono::high_resolution_clock::now();
    }

    void SignalTimer::reset()
    {
        m_running = false;
        m_start = std::chrono::high_resolution_clock::now();
        m_elapsed = std::chrono::high_resolution_clock::now();
    }

    double SignalTimer::elapsed() const
    {
        if (m_running)
            return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - m_start).count();
        else
            return std::chrono::duration_cast<std::chrono::duration<double>>(m_elapsed - m_start).count();
    }

    bool SignalTimer::check()
    {
        bool result = elapsed() >= m_seconds;
        if(result && elapsed() >= m_seconds + m_secondsActive)
            m_start = std::chrono::high_resolution_clock::now();

        return result;
    }
}