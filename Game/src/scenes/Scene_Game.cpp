#include "Scene_Game.hpp"
#include "Action.hpp"
#include "Buttons.hpp"
#include "SFML/System/Angle.hpp"
#include "SFML/System/Vector2.hpp"
#include "scene base/Components.hpp"
#include "scene base/TileEffect.hpp"
#include "scenes/Scene_MainMenu.hpp"

#include <algorithm>
#include <iostream>
#include <memory>

void Scene_Game::init()
{
    m_map.init({1280, 720}, {32, 32}, &m_assets);
    registerAction(Buttons::L, static_cast<int>(ActionTypes::loadLevel));

    registerAction(Buttons::W, static_cast<int>(ActionTypes::up));
    registerAction(Buttons::A, static_cast<int>(ActionTypes::left));
    registerAction(Buttons::S, static_cast<int>(ActionTypes::down));
    registerAction(Buttons::D, static_cast<int>(ActionTypes::right));

    m_assets.loadTextureDir("tiles");
    registerTexture("Player", "player.png");
    registerTexture("Spawner", "spawner.png");
    registerTexture("Enemy", "enemy.png");
    registerTexture("Target", "target.png");
}

void Scene_Game::onExit()
{
    m_game->startScene(std::make_shared<Scene_MainMenu>(m_game));
}

void Scene_Game::update()
{
    sMovement();
    sSprites();

    sSpawners();

    sRender();
}

void Scene_Game::sDoAction(const Action& action)
{
    switch(static_cast<ActionTypes>(action.type()))
    {
    case ActionTypes::up:
        if(action.status() == Action::start)
        {
            m_entities.getComponent<cInput>(m_player).up = true;
        }
        else if(action.status() == Action::end)
        {
            m_entities.getComponent<cInput>(m_player).up = false;
        }
        break;

    case ActionTypes::left:
        if(action.status() == Action::start)
        {
            m_entities.getComponent<cInput>(m_player).left = true;
        }
        else if(action.status() == Action::end)
        {
            m_entities.getComponent<cInput>(m_player).left = false;
        }
        break;

    case ActionTypes::down:
        if(action.status() == Action::start)
        {
            m_entities.getComponent<cInput>(m_player).down = true;
        }
        else if(action.status() == Action::end)
        {
            m_entities.getComponent<cInput>(m_player).down = false;
        }
        break;

    case ActionTypes::right:
        if(action.status() == Action::start)
        {
            m_entities.getComponent<cInput>(m_player).right = true;
        }
        else if(action.status() == Action::end)
        {
            m_entities.getComponent<cInput>(m_player).right = false;
        }
        break;

    case ActionTypes::endScene:
        m_hasEnded = true;
        break;

    case ActionTypes::exit:
        m_game->quit();
        break;

    case ActionTypes::loadLevel:
        if(action.status() == Action::start)
        {

            loadLevel();
        }
        break;
    }
}

void Scene_Game::sRender()
{
    auto& window = m_game->getWindow();
    window.beginDraw();

    sf::Sprite mapSpr{m_map.getTexture()};
    window.draw(mapSpr);

    for(auto& ent : m_entities.getEntities<cSprite>())
    {
        auto& spr = m_entities.getComponent<cSprite>(ent);
        m_game->getWindow().draw(spr.sprite);
    }

    window.render();
}

void Scene_Game::sSpawners()
{
    //WARN: debug code
    for(auto& ent : m_entities.getEntities<cSpawner>())
    {
        auto& spawner = m_entities.getComponent<cSpawner>(ent);

        if(spawner.waveTimer == 0)
        {
            if(spawner.waveSpawnInterval == 0)
            {
                if(spawner.enemiesToSpawn > 0)
                {
                    spawnEnemy({0, 0});
                    spawner.enemiesToSpawn--;
                    spawner.waveSpawnInterval = 60;
                }
                else
                {
                    spawner.waveTimer = 300;
                    spawner.enemiesToSpawn = 3;
                }
            }
            else
            {
                spawner.waveSpawnInterval--;
            }
        }
        else
        {
            spawner.waveTimer--;
        }
    }
}

void Scene_Game::sSprites()
{
    for(auto& ent : m_entities.getEntities<cSprite, cTransform>())
    {
        auto& sprite = m_entities.getComponent<cSprite>(ent).sprite;
        auto& transform = m_entities.getComponent<cTransform>(ent);

        sprite.setPosition(transform.pos);
        sprite.setScale(transform.scale);
        sprite.setRotation(transform.angle);
    }
}

void Scene_Game::sMovement()
{
    const float moveSpeed{5.f};

    for(auto& ent : m_entities.getEntities<cInput, cTransform>())
    {
        auto& velocity = m_entities.getComponent<cTransform>(ent).vel;
        auto& input = m_entities.getComponent<cInput>(ent);

        if(input.up)
        {
            velocity.y -= moveSpeed;
        }

        if(input.down)
        {
            velocity.y += moveSpeed;
        }

        if(input.left)
        {
            velocity.x -= moveSpeed;
        }

        if(input.right)
        {
            velocity.x += moveSpeed;
        }

        // clamp the speed if we are pressing muliple buttons
        float speed = std::abs(velocity.x) + std::abs(velocity.y);
        if(speed > moveSpeed + (moveSpeed / 2.0f))
        {
            velocity.x = std::clamp(
                velocity.x, (-moveSpeed / 1.5f), (moveSpeed / 1.5f));
            velocity.y = std::clamp(
                velocity.y, (-moveSpeed / 1.5f), (moveSpeed / 1.5f));
        }

        velocity.x *= 0.8f;
        velocity.y *= 0.8f;
    }

    for(auto& ent : m_entities.getEntities<cTransform>())
    {
        auto& transform = m_entities.getComponent<cTransform>(ent);
        transform.pos += transform.vel;
    }
}

void Scene_Game::spawnPlayer(const sf::Vector2f& pos)
{
    auto ent = m_entities.addEntity();
    m_player = ent;

    auto& transform = m_entities.addComponent<cTransform>(ent);
    auto& sprite = m_entities.addComponent<cSprite>(ent).sprite;
    m_entities.addComponent<cInput>(ent);

    transform.pos = pos;

    sprite.setTexture(m_assets.getTexture("Player"), true);
    auto& tex = sprite.getTexture();
    sprite.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});
}

void Scene_Game::spawnSpawner(const sf::Vector2f& pos)
{
    auto ent = m_entities.addEntity();

    auto& transform = m_entities.addComponent<cTransform>(ent);
    auto& sprite = m_entities.addComponent<cSprite>(ent).sprite;
    auto& spawner = m_entities.addComponent<cSpawner>(ent);

    transform.pos = pos;

    sprite.setTexture(m_assets.getTexture("Spawner"), true);
    auto& tex = sprite.getTexture();
    sprite.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});

    spawner.enemiesToSpawn = 3;
    spawner.waveSpawnInterval = 60;
    spawner.waveTimer = 300;
}

void Scene_Game::spawnEnemy(const sf::Vector2f& pos)
{
    auto ent = m_entities.addEntity();

    auto& transform = m_entities.addComponent<cTransform>(ent);
    auto& sprite = m_entities.addComponent<cSprite>(ent).sprite;

    transform.pos = pos;
    transform.vel.x = 5;

    sprite.setTexture(m_assets.getTexture("Enemy"), true);
    auto& tex = sprite.getTexture();
    sprite.setOrigin({tex.getSize().x / 2.f, tex.getSize().y / 2.f});
}

void Scene_Game::spawnTarget(const sf::Vector2f& pos) {}

void Scene_Game::loadLevel()
{
    m_map.load("../../../Game/res/saves/PathfindingTest.sff");

    // convert tile effects to entities

    for(auto& tile : m_map.getTiles())
    {
        if(!tile->effects.empty())
        {
            auto tileWorldPos = m_map.toWorldPos(tile->pos);
            for(auto& effect : tile->effects)
            {
                if(effect.type == TileEffect::spawner)
                {
                    spawnSpawner(tileWorldPos);
                }
                // WARN: water is being used as a stand in for target
                else if(effect.type == TileEffect::water)
                {
                    spawnPlayer(tileWorldPos);
                    spawnTarget(tileWorldPos);
                }
            }
        }
    }
}

