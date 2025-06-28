#pragma once

#include "Action.hpp"
#include "Assets.hpp"
#include "Buttons.hpp"
#include "DebugUI.hpp"
#include "EntityManager.hpp"
#include "GameEngine.hpp"

#include <filesystem>
#include <map>

class GameEngine;

class Scene
{
public:
    Scene(GameEngine* game)
        : m_game{game}, m_debugUI{m_game->getDT(),
                                  m_game->getMaxFps(),
                                  m_game->getWindow(),
                                  m_assets,
                                  m_entities}
    {
        m_debugUI.init(m_game->getWindow());
    }

    virtual void update() = 0;
    virtual void sDoAction(const Action& action) = 0;
    virtual void sRender() = 0;
    virtual void init() = 0;

    // end behavior, maybe we want to open another scene on closing
    virtual void onExit() {}

    bool hasEnded() const { return m_hasEnded; }

    void registerAction(Buttons::Button button, int typeIndex);

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

    const std::map<Buttons::Button, int>& getActions() const
    {
        return m_actions;
    }

    virtual ~Scene() {};

protected:
    GameEngine* m_game{};
    SECS::EntityManager m_entities;
    Assets m_assets;
    std::map<Buttons::Button, int> m_actions;

    // Indicates to the game engine that the scene is over, next game loop, it
    // will be deleted
    bool m_hasEnded{false};
    bool m_paused{false};

    // Whether to show/update the debug UI
    bool m_debug;
    DebugUI m_debugUI;
};
