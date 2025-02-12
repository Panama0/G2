#pragma once

#include "GameEngine.hpp"
#include "EntityManager.hpp"
#include "Action.hpp"
#include "Assets.hpp"
#include <map>
#include <string>
#include <memory>

class GameEngine;

class Scene
{
public:
    virtual void update() = 0;
    virtual void sDoAction() = 0;
    virtual void sRender() = 0;
    
    void simulate(int iterations);
    void doAction(const Action& a);
    
    enum class AssetType
    {
        font,
        texture,
        animation,
        sound
    };
    void registerAsset(AssetType type, std::string name, std::string path);
    void registerAction(const Action& a);
private:
    std::unique_ptr<GameEngine> m_game;
    EntityManager m_entities;
    int m_currentFrame {};
    std::vector<std::string> m_assetNames;
    Assets m_assets;
    std::map<Action, std::string> m_actionMap;
    bool m_paused {false};
    bool m_hasEnded {false};
};