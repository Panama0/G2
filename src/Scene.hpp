#pragma once

#include "GameEngine.hpp"
#include "EntityManager.hpp"
#include "Action.hpp"
#include "Assets.hpp"
#include <map>
#include <string>
#include <memory>
#include <filesystem>

class GameEngine;

class Scene
{
public:
    Scene() = default;
    Scene(GameEngine* game)
        :m_game {game}
    {}
    enum SceneTypes
    {
        mainmenu,
        gameplay
    };

    virtual void update() = 0;
    virtual void sDoAction(const Action& action) = 0;
    virtual void sRender() = 0;
    virtual void init() = 0;
    
    void simulate(int iterations);
    void doAction(const Action& a);
    
    void registerAction(sf::Keyboard::Key key, int index);

    void registerFont(std::string_view name, const std::filesystem::path& fname);
    void registerTexture(std::string_view name, const std::filesystem::path& fname);
    void registerAnimation(std::string_view name, const std::filesystem::path& fname, uint32_t frames, uint32_t interval);
    void registerSound(std::string_view name, const std::filesystem::path& fname);
    
    const std::map<sf::Keyboard::Key, int>& getActionMap() const { return m_actionMap; }
protected:
    GameEngine* m_game;
    EntityManager m_entities;
    int m_currentFrame {};
    std::vector<std::string> m_assetNames;
    Assets m_assets;
    std::map<sf::Keyboard::Key, int> m_actionMap;
    bool m_paused {false};
    bool m_hasEnded {false};
};