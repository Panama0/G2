#pragma once

#include "Defines.hpp"
#include "SFML/System/Vector2.hpp"
#include "Scene.hpp"
#include "scene base/GameMap.hpp"

class Scene_Game : public Scene
{
public:
    enum class ActionTypes
    {
        up,
        left,
        down,
        right,
        loadLevel,
        exit,
        endScene,
    };

    Scene_Game() = default;
    Scene_Game(GameEngine* game) : Scene(game) { init(); }

    void init() override;
    void onExit() override;
    void update() override;
    void sDoAction(const Action& action) override;
    void sRender() override;

private:

    void sSpawners();
    void sPathfinding();
    void sSprites();
    void sMovement();
    void sCollision();

    void spawnPlayer(const sf::Vector2f& pos);
    void spawnSpawner(const sf::Vector2f& pos);
    void spawnEnemy(const sf::Vector2f& pos);
    void spawnTarget(const sf::Vector2f& pos);

    void loadLevel();

    SECS::Entity m_player{};

    GameMap m_map;
};

