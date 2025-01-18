#include "eutil/scene/scene.hpp"

namespace util
{
    Scene::Scene()
    : runUpdate(true), runRender(true), runEvent(true)
    {}

    bool Scene::isRunningUpdate() const
    {
        return runUpdate;
    }

    bool Scene::isRunningRender() const
    {
        return runRender;
    }

    bool Scene::isRunningEvent() const
    {
        return runEvent;
    }

    void Scene::setRunUpdate(bool run)
    {
        runUpdate = run;
    }

    void Scene::setRunRender(bool run)
    {
        runRender = run;
    }

    void Scene::setRunEvent(bool run)
    {
        runEvent = run;
    }

    void Scene::pushEvent(Event* event)
    {
        eventQueue->push(event);
    }

    void Scene::setEventQueue(std::shared_ptr<std::queue<Event*>> eQueue)
    {
        eventQueue = eQueue;
    }
}