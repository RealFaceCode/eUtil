#pragma once

#include <chrono>

#include "defines.hpp"

namespace eutil
{
    class EUTIL_API Timer
    {
    public:
            EUTIL_API Timer();
            EUTIL_API ~Timer();

            EUTIL_API void start();
            EUTIL_API void stop();
            EUTIL_API void reset();

            EUTIL_API double elapsed() const;

        private:
            bool m_running = false;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_elapsed;
    };
}
