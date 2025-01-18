#include "eutil/scene/scenemanager.hpp"

namespace eutil
{
    SceneManager::SceneManager(std::shared_ptr<std::queue<Event*>> eQueue)
    : eventQueue(eQueue)
    {}

    void SceneManager::PushScene(const std::string& sceneName, Scene& scene)
    {
        auto it = scenes.find(sceneName);
        if (it != scenes.end())
            return;
        
        scenes[sceneName] = &scene;
    }

    void SceneManager::PushScene(const std::string& sceneName, Scene* scene)
    {
        auto it = scenes.find(sceneName);
        if (it != scenes.end())
            return;
        
        scenes[sceneName] = scene;
        scene->setEventQueue(eventQueue);
        scene->onAttach();
    }

    void SceneManager::PopScene(const std::string& sceneName)
    {
        auto it = scenes.find(sceneName);
        if (it == scenes.end())
            return;

        it->second->onDetach();
        scenes.erase(it);
    }

    Scene* SceneManager::getScene(const std::string& sceneName)
    {
        auto it = scenes.find(sceneName);
        if (it == scenes.end())
            return nullptr;

        return it->second;
    }

    void SceneManager::pauseUpdateScene(const std::string& sceneName)
    {
        auto it = scenes.find(sceneName);
        if (it == scenes.end())
            return;

        it->second->setRunUpdate(false);
    }

    void SceneManager::resumeUpdateScene(const std::string& sceneName)
    {
        auto it = scenes.find(sceneName);
        if (it == scenes.end())
            return;

        it->second->setRunUpdate(true);
    }

    void SceneManager::pauseRenderScene(const std::string& sceneName)
    {
        auto it = scenes.find(sceneName);
        if (it == scenes.end())
            return;

        it->second->setRunRender(false);
    }

    void SceneManager::resumeRenderScene(const std::string& sceneName)
    {
        auto it = scenes.find(sceneName);
        if (it == scenes.end())
            return;

        it->second->setRunRender(true);
    }

    void SceneManager::pauseEventScene(const std::string& sceneName)
    {
        auto it = scenes.find(sceneName);
        if (it == scenes.end())
            return;

        it->second->setRunEvent(false);
    }

    void SceneManager::resumeEventScene(const std::string& sceneName)
    {
        auto it = scenes.find(sceneName);
        if (it == scenes.end())
            return;

        it->second->setRunEvent(true);
    }

    void SceneManager::runScenesUpdate()
    {
        for (auto& scene : scenes)
        {
            if (scene.second->isRunningUpdate())
                scene.second->onUpdate();
        }
    }

    void SceneManager::runScenesRender()
    {
        for (auto& scene : scenes)
        {
            if (scene.second->isRunningRender())
                scene.second->onRender();
        }
    }

    void SceneManager::runScenesEvent(Event& event)
    {
        for (auto& scene : scenes)
        {
            if (scene.second->isRunningEvent())
                scene.second->onEvent(event);
        }
    }

    void SceneManager::setEventQueue(std::shared_ptr<std::queue<Event*>> eQueue)
    {
        eventQueue = eQueue;
    }
}