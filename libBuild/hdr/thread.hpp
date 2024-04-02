#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>
#include <atomic>
#include <memory>
#include <future>
#include <iostream>

#include "defines.hpp"

namespace eutil
{
    template<typename DATA, typename RESULT = int>
    struct EUTIL_API Thread
    {
    public:
        using TaskData = DATA;
        using TaskResult = RESULT;
        using TaskFunc = std::optional<TaskResult> (*)(const TaskData&);

        EUTIL_APIThread() = default;
        EUTIL_APIexplicit Thread(TaskFunc func) : m_func(func) {}
        EUTIL_API~Thread() { stop(); }

        EUTIL_APIvoid start()
        {
            if(!m_queue)
                m_queue = std::make_shared<std::queue<TaskData>>();
            if(!m_results)
                m_results = std::make_shared<std::vector<TaskResult>>();
            if(!m_mutex)
                m_mutex = std::make_shared<std::mutex>();
            if(!m_resultMutex)
                m_resultMutex = std::make_shared<std::mutex>();
            if(!m_cv)
                m_cv = std::make_shared<std::condition_variable>();

            m_thread = std::jthread([this](std::stop_token stoken)
            {
                std::queue<TaskData>& queue = *m_queue.get();
                std::vector<TaskResult>& results = *m_results.get();

                while (!stoken.stop_requested())
                {
                    TaskData data;
                    {
                        std::unique_lock<std::mutex> lock(*m_mutex);
                        m_cv->wait(lock, [this, &stoken, &queue]
                        {
                            return stoken.stop_requested() || !queue.empty();
                        });

                        if (stoken.stop_requested())
                            break;

                        data = queue.front();
                        queue.pop();
                    }

                    auto result = m_func(data);
                    if(result)
                    {
                        std::unique_lock<std::mutex> lock(*m_resultMutex);
                        results.emplace_back(result.value());
                    }
                }
            });
        }

        EUTIL_APIvoid stop()
        {
            if (m_thread.joinable())
            {
                m_thread.request_stop();
                if(m_cv)
                    m_cv->notify_one();
                m_thread.join();
            }
        }

        EUTIL_APIvoid join()
        {
            if (m_thread.joinable())
                m_thread.join();
        }

        EUTIL_APIvoid push(const TaskData& data)
        {
            if(!m_mutex)
                return;

            std::unique_lock<std::mutex> lock(*m_mutex);
            if(m_queue)
                m_queue.get()->push(data);
            if(m_cv)
                m_cv->notify_one();
        }

        EUTIL_APIvoid setFunc(TaskFunc func) { m_func = func; }

        EUTIL_APIstd::optional<std::vector<TaskResult>> get()
        {
            if(!m_resultMutex)
                return std::nullopt;

            std::unique_lock lock(*m_resultMutex);
            if (m_results.get()->empty())
                return std::nullopt;

            return *m_results.get();
        }

        EUTIL_APIvoid setQueue(std::shared_ptr<std::queue<TaskData>> queue) { m_queue = queue; }
        EUTIL_APIvoid setResults(std::shared_ptr<std::vector<TaskResult>> results) { m_results = results; }
        EUTIL_APIvoid setMutex(std::shared_ptr<std::mutex> mutex) { m_mutex = mutex; }
        EUTIL_APIvoid setResultMutex(std::shared_ptr<std::mutex> mutex) { m_resultMutex = mutex; }
        EUTIL_APIvoid setCV(std::shared_ptr<std::condition_variable> cv) { m_cv = cv; }

        EUTIL_APIstd::jthread& getThread() { return m_thread; }

        private:
            std::jthread m_thread;
            std::shared_ptr<std::mutex> m_mutex;
            std::shared_ptr<std::mutex> m_resultMutex;
            std::shared_ptr<std::condition_variable> m_cv;
            std::shared_ptr<std::queue<TaskData>> m_queue = nullptr;
            std::shared_ptr<std::vector<TaskResult>> m_results = nullptr;
            TaskFunc m_func = nullptr;
    };
}