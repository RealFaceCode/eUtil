#pragma once

#include "eutil/defines.hpp"
#include "scene.hpp"

#include <unordered_map>
#include <string>
#include <queue>
#include <memory>

namespace util
{
    class EUTIL_API SceneManager
    {
    public:
        SceneManager() = default;
        explicit SceneManager(std::shared_ptr<std::queue<Event*>> eQueue);
        ~SceneManager() = default;

        void PushScene(const std::string& sceneName, Scene& scene);
        void PushScene(const std::string& sceneName, Scene* scene);
        void PopScene(const std::string& sceneName);
        Scene* getScene(const std::string& sceneName);

        void pauseUpdateScene(const std::string& sceneName);
        void resumeUpdateScene(const std::string& sceneName);
        void pauseRenderScene(const std::string& sceneName);
        void resumeRenderScene(const std::string& sceneName);
        void pauseEventScene(const std::string& sceneName);
        void resumeEventScene(const std::string& sceneName);

        void runScenesUpdate();
        void runScenesRender();
        void runScenesEvent(Event& event);

        void setEventQueue(std::shared_ptr<std::queue<Event*>> eQueue);

    private:
        std::unordered_map<std::string, Scene*> scenes;
        std::shared_ptr<std::queue<Event*>> eventQueue;
    };
}