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
    Scene() = default;
    Scene(GameEngine* game)
        :m_game {game}
    {}

    virtual void update() = 0;
    virtual void sDoAction(const Action& action) = 0;
    virtual void sRender() = 0;
    virtual void init() = 0;
    
    void simulate(int iterations);
    void doAction(const Action& a);
    
    enum class AssetType
    {
        font,
        texture,
        animation,
        sound
    };
    //* not 100 percent sure this is a good way to solve this
    void registerAsset(AssetType type, std::string name, std::string path, uint32_t frames = 0, uint32_t interval = 0);
    void registerAction(sf::Keyboard::Key key, int index);
    
    const std::map<sf::Keyboard::Key, int>& getActionMap() const { return m_actionMap; }
protected:
    std::shared_ptr<GameEngine> m_game;
    EntityManager m_entities;
    int m_currentFrame {};
    std::vector<std::string> m_assetNames;
    Assets m_assets;
    std::map<sf::Keyboard::Key, int> m_actionMap;
    bool m_paused {false};
    bool m_hasEnded {false};
};