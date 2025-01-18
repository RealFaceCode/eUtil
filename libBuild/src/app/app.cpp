#include "eutil/app/app.hpp"

#include "eutil/event/event.hpp"
#include "eutil/event/eventdispatcher.hpp"
#include "eutil/scene/scene.hpp"

namespace eutil
{
    void App::setUpdateInterval(double interval)
    {
        uInterval = 1 / interval;
    }

    void App::setRenderInterval(double interval)
    {
        rInterval = 1 / interval;
    }

    void App::useUpdateInterval(bool updateInterval)
    {
        useUInterval = updateInterval;
    }

    void App::useRenderInterval(bool renderInterval)
    {
        useRInterval = renderInterval;
    }

    void App::setIntervals(double updateInterval, double renderInterval)
    {
        setUpdateInterval(updateInterval);
        setRenderInterval(renderInterval);
    }

    void App::useIntervals(bool updateInterval, bool renderInterval)
    {
        useUpdateInterval(updateInterval);
        useRenderInterval(renderInterval);
    }

    void App::setIntervals(double updateInterval, double renderInterval, bool useUpdateInterval, bool useRenderInterval)
    {
        setIntervals(updateInterval, renderInterval);
        useIntervals(useUpdateInterval, useRenderInterval);
    }

    void App::run(const std::string& initialSceneName, Scene* initialScene)
    {
        running = true;
        eventQueue = std::make_shared<std::queue<Event*>>();
        sceneManager.setEventQueue(eventQueue);

        if(useUInterval)
        {
            updateTimer.setSeconds(uInterval);
            updateTimer.start();
        }

        if(useRInterval)
        {
            renderTimer.setSeconds(rInterval);
            renderTimer.start();
        }

        onStart();

        sceneManager.PushScene(initialSceneName, initialScene);

        while (running)
        {
            while(!eventQueue->empty())
            {
                auto event = eventQueue->front();
                eventQueue->pop();

                interOnEvent(*event);
                sceneManager.runScenesEvent(*event);

                delete event;
            }

            // Maybe use events for update and render
            if(updateTimer.check() || !useUInterval)
                sceneManager.runScenesUpdate();
            if(renderTimer.check() || !useRInterval)
                sceneManager.runScenesRender();
        }

        onStop();
    }

    void App::pushEvent(Event* event)
    {
        eventQueue->push(event);
    }

    void App::interOnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<ScenePushEvent>([this](ScenePushEvent& e)
        {
            sceneManager.PushScene(e.name, e.scene);
            return true;
        });

        dispatcher.dispatch<ScenePopEvent>([this](ScenePopEvent& e)
        {
            sceneManager.PopScene(e.name);
            return true;
        });

        onEvent(event);
    }
}