#pragma once

#include "thread.hpp"
#include "defines.hpp"

namespace eutil
{
    template<typename DATA, typename RESULT = int>
    struct EUTIL_API ThreadPool
    {
    public:
        using TaskData = DATA;
        using TaskResult = RESULT;
        using TaskFunc = Thread<TaskData, TaskResult>::TaskFunc;

        ThreadPool() = default;
        explicit ThreadPool(TaskFunc func, size_t size = 1) 
        : m_func(func)
        {
            for (size_t i = 0; i < size; ++i)
                m_threads.emplace_back(std::make_shared<Thread<TaskData, TaskResult>>(m_func));
        }
        ~ThreadPool() = default;

        void setTaskFunc(TaskFunc func)
        {
            m_func = func;
            for (auto thread : m_threads)
                thread->setTaskFunc(m_func);
        }

        void setThreadCount(size_t size)
        {
            m_threads.clear();
            for (size_t i = 0; i < size; ++i)
                m_threads.emplace_back(std::make_shared<Thread<TaskData, TaskResult>>(m_func));
        }

        void start()
        {
            for (auto thread : m_threads)
            {
                thread->setMutex(m_mutex);
                thread->setResultMutex(m_resultMutex);
                thread->setCV(m_cv);
                thread->setQueue(m_queue);
                thread->setResults(m_results);
                thread->start();
            }
        }

        void stop()
        {
            for (auto& thread : m_threads)
            {
                auto& t = thread->getThread();
                if (t.joinable())
                    t.request_stop();
            }
            m_cv->notify_all();
            join();
        }

        void join()
        {
            for (auto& thread : m_threads)
                thread->join();
        }

        void push(const TaskData& data)
        {
            std::unique_lock lock(*m_mutex);
            m_queue->push(data);
            m_cv->notify_one();
        }

        std::optional<std::vector<TaskResult>> get()
        {
            if(!m_resultMutex)
                return std::nullopt;

            std::unique_lock lock(*m_resultMutex);
            if (m_results.get()->empty())
                return std::nullopt;

            return *m_results.get();
        }

    private:
        std::vector<std::shared_ptr<Thread<TaskData, TaskResult>>> m_threads;
        std::shared_ptr<std::mutex> m_mutex                 = std::make_shared<std::mutex>();
        std::shared_ptr<std::mutex> m_resultMutex           = std::make_shared<std::mutex>();
        std::shared_ptr<std::condition_variable> m_cv       = std::make_shared<std::condition_variable>();
        std::shared_ptr<std::queue<TaskData>> m_queue       = std::make_shared<std::queue<TaskData>>();
        std::shared_ptr<std::vector<TaskResult>> m_results  = std::make_shared<std::vector<TaskResult>>();
        TaskFunc m_func;
    };
}