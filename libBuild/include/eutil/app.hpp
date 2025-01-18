#pragma once

#include "defines.hpp"
#include "scene/scene.hpp"
#include "scene/scenemanager.hpp"
#include "eutil/signaltimer.hpp"

#include <queue>
#include <memory>

namespace eutil
{
    struct Event;

    class EUTIL_API App
    {
    public:
        App() = default;
        virtual ~App() = default;

        virtual void onStart() = 0;
        virtual void onEvent(Event& event) = 0;
        virtual void onStop() = 0;

        void setUpdateInterval(double interval);
        void setRenderInterval(double interval);
        void useIntervalUpdate(bool updateInterval);
        void useIntervalRender(bool renderInterval);

        virtual void run(const std::string& initialSceneName, Scene* initialScene);
        void pushEvent(Event* event);

    private:
        bool running;
        SceneManager sceneManager;
        std::shared_ptr<std::queue<Event*>> eventQueue;
        double updateInterval;
        double renderInterval;
        bool useUpdateInterval;
        bool useRenderInterval;
        SignalTimer updateTimer;
        SignalTimer renderTimer;
    };
}