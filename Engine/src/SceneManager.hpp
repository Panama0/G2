#pragma once

#include <memory>
#include <vector>

class Scene;

class SceneManager
{
public:
    void start(std::shared_ptr<Scene> scene);
    void update();

    Scene* getCurrentScene();
    bool empty();

private:
    std::vector<std::shared_ptr<Scene>> m_scenes;
    Scene* m_currentScene{};
    std::shared_ptr<Scene> m_nextScene;
};
