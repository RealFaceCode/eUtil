#include "signaltimer.hpp"

namespace eutil
{
    SignalTimer::SignalTimer()
        : m_running(false)
    {}

    SignalTimer::SignalTimer(double seconds)
        : m_running(false), m_seconds(seconds)
    {}

    SignalTimer::~SignalTimer() = default;

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
        if(result)
            m_start = std::chrono::high_resolution_clock::now();
        return result;
    }
}