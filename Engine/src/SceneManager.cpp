#include "SceneManager.hpp"
#include "Scene.hpp"

Scene* SceneManager::getCurrentScene() { return m_currentScene; }

bool SceneManager::empty() { return m_scenes.empty(); }

void SceneManager::start(std::shared_ptr<Scene> scene)
{
    if(m_nextScene)
    {
        std::cerr << "Tried to create multiple scenes!\n";
    }
    m_nextScene = scene;
}

void SceneManager::update()
{
    // delete the old scene
    if(m_currentScene->hasEnded())
    {
        // run closing code
        m_currentScene->onExit();

        // find and destroy the scene
        for(auto it = m_scenes.begin(); it != m_scenes.end(); it++)
        {
            if(it->get() == m_currentScene)
            {
                m_scenes.erase(it);
                // avoid iterator invalidation
                break;
            }
        }
    }

    // start the new scene
    if(m_nextScene)
    {
        m_scenes.push_back(m_nextScene);
        m_currentScene = m_scenes.back().get();
        m_nextScene = nullptr;
    }
}
