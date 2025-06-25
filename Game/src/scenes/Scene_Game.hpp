#pragma once

#include "Defines.hpp"
#include "Scene.hpp"
#include "Vec2.hpp"
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

    void spawnPlayer(const Vec2f& pos);
    void spawnSpawner(const Vec2f& pos);
    void spawnEnemy(const Vec2f& pos);
    void spawnTarget(const Vec2f& pos);

    void loadLevel();

    SECS::Entity m_player{};

    GameMap m_map;
};
