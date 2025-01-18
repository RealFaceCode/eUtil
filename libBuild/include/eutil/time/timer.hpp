#pragma once

#include <chrono>

#include "eutil/defines.hpp"

namespace util
{
    class EUTIL_API Timer
    {
    public:
            Timer();
            ~Timer();

            void start();
            void stop();
            void reset();

            double elapsed() const;

        private:
            bool m_running = false;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_elapsed;
    };
}
