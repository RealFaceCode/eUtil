#include "eutil/app.hpp"

namespace eutil
{
    void App::setUpdateInterval(double interval)
    {
        updateInterval = interval / 1000.0;
    }

    void App::setRenderInterval(double interval)
    {
        renderInterval = interval / 1000.0;
    }

    void App::useIntervalUpdate(bool updateInterval)
    {
        useUpdateInterval = updateInterval;
    }

    void App::useIntervalRender(bool renderInterval)
    {
        useRenderInterval = renderInterval;
    }

    void App::run(const std::string& initialSceneName, Scene* initialScene)
    {
        running = true;
        eventQueue = std::make_shared<std::queue<Event*>>();
        sceneManager.setEventQueue(eventQueue);

        if(useUpdateInterval)
        {
            updateTimer.setSeconds(updateInterval);
            updateTimer.start();
        }
        if(useRenderInterval)
        {
            renderTimer.setSeconds(renderInterval);
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

                onEvent(*event);
                sceneManager.runScenesEvent(*event);

                delete event;
            }

            // Maybe use events for update and render
            if(updateTimer.check())
                sceneManager.runScenesUpdate();
            if(renderTimer.check())
                sceneManager.runScenesRender();
        }

        onStop();
    }

    void App::pushEvent(Event* event)
    {
        eventQueue->push(event);
    }
}