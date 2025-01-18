#pragma once

#include "eutil/defines.hpp"
#include "eutil/event/event.hpp"

#include <queue>
#include <memory>

namespace util
{
    class Scene;

    struct ScenePushEvent : public Event
    {
    public:
        ScenePushEvent(const std::string& name, Scene* scene)
        : name(name), scene(scene) {}

        std::string name;
        Scene* scene;
    };

    struct ScenePopEvent : public Event
    {
    public:
        explicit ScenePopEvent(const std::string& name)
        : name(name) {}

        std::string name;
    };

    class EUTIL_API Scene
    {
    public:
        Scene();
        virtual ~Scene() = default;

        virtual void onAttach() = 0;
        virtual void onDetach() = 0;
        virtual void onUpdate() = 0;
        virtual void onRender() = 0;
        virtual void onEvent(Event& event) = 0;

        bool isRunningUpdate() const;
        bool isRunningRender() const;
        bool isRunningEvent() const;
        void setRunUpdate(bool run);
        void setRunRender(bool run);
        void setRunEvent(bool run);

        void pushEvent(Event* event);
        void setEventQueue(std::shared_ptr<std::queue<Event*>> eQueue);

    private:
        bool runUpdate;
        bool runRender;
        bool runEvent;
        std::shared_ptr<std::queue<Event*>> eventQueue;
    };
}