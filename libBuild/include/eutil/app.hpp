#pragma once

#include "defines.hpp"
#include "scene/scenemanager.hpp"
#include "eutil/signaltimer.hpp"

#include <queue>
#include <memory>

namespace eutil
{
    struct Event;
    class Scene;

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
        void useUpdateInterval(bool updateInterval);
        void useRenderInterval(bool renderInterval);
        void setIntervals(double updateInterval, double renderInterval);
        void useIntervals(bool updateInterval, bool renderInterval);
        void setIntervals(double updateInterval, double renderInterval, bool useUpdateInterval, bool useRenderInterval);

        virtual void run(const std::string& initialSceneName, Scene* initialScene);
        void pushEvent(Event* event);

    private:
        void interOnEvent(Event& event);
        bool running;
        SceneManager sceneManager;
        std::shared_ptr<std::queue<Event*>> eventQueue;
        double uInterval    = 0;
        double rInterval    = 0;
        bool useUInterval   = false;
        bool useRInterval   = false;
        SignalTimer updateTimer;
        SignalTimer renderTimer;
    };
}