#pragma once

#include "Action.hpp"
#include "EntityManager.hpp"
#include "GameEngine.hpp"
#include "Assets.hpp"
#include "Grid.hpp"

#include <filesystem>
#include <map>
#include <string>

class GameEngine;

class Scene
{
public:
    Scene() = default;
    Scene(GameEngine* game, uint32_t id) : m_game{game}, m_id(id) {}
    enum SceneTypes
    {
        mainmenu,
        editor,
        gameplay
    };

    virtual void update() = 0;
    virtual void sDoAction(const Action& action) = 0;
    virtual void sRender() = 0;
    virtual void init() = 0;

    bool hasGUI() { return m_hasGui; }

    virtual void end() = 0;
    bool hasEnded() { return m_hasEnded; }
    uint32_t id() { return m_id; }

    void simulate(int iterations);

    void registerAction(sf::Keyboard::Key key, int index);
    void registerAction(sf::Mouse::Button button, int index);

    void registerFont(std::string_view name,
                      const std::filesystem::path& fname);
    void registerTexture(std::string_view name,
                         const std::filesystem::path& fname);
    void registerAnimation(std::string_view name,
                           const std::filesystem::path& fname,
                           uint32_t frames,
                           uint32_t interval);
    void registerSound(std::string_view name,
                       const std::filesystem::path& fname);

    const std::map<sf::Keyboard::Key, int>& getKeyboardActions() const
    {
        return m_keyboardActions;
    }
    const std::map<sf::Mouse::Button, int>& getMouseActions() const
    {
        return m_mouseActions;
    }

    virtual ~Scene() {};

protected:
    GameEngine* m_game;
    uint32_t m_id{};
    EntityManager m_entities;
    int m_currentFrame{};
    Assets m_assets;
    std::map<sf::Keyboard::Key, int> m_keyboardActions;
    std::map<sf::Mouse::Button, int> m_mouseActions;
    bool m_paused{false};
    bool m_hasEnded{false};
    bool m_hasGui{false};

    Grid m_globalGrid;
};