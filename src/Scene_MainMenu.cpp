#include "Scene_MainMenu.hpp"

#include <iostream>

void Scene_MainMenu::update()
{
    m_view = m_game->getWindow().getView();
    m_entities.update();
    
    for(auto& entity : m_entities.getEntities())
    {
        if(entity->has<cTransform>() && entity->has<cSprite>())
        {
            auto& spr = entity->get<cSprite>().sprite;
            const auto& transform = entity->get<cTransform>();
            spr.setPosition(transform.pos);
            spr.setScale(transform.scale);
        }
    }
    sAnimation();
    sRender();
}

void Scene_MainMenu::sDoAction(const Action& action)
{
    switch(action.type())
    {
        case ActionTypes::FS:
            if(action.status() == Action::end) { m_game->getWindow().toggleFullscreen(); };
            break;
    }
}

void Scene_MainMenu::sRender()
{
    m_game->getWindow().beginDraw();
    
    
    for(auto& entity : m_entities.getEntities())
    {
        if(entity->has<cSprite>() && entity->isActive())
        {
            m_game->getWindow().draw(entity->get<cSprite>().sprite);
        }
    }
    sf::Sprite grid {m_game->getGrid().getTexture()};
    m_game->getWindow().draw(grid);
    m_game->getWindow().render();
}

void Scene_MainMenu::sAnimation()
{
    for(auto& entity : m_entities.getEntities())
    {
        if(entity->has<cAnimation>() && entity->has<cSprite>())
        {
            auto& animationC = entity->get<cAnimation>();
            if(animationC.animation.hasEnded())
            {
                if(!animationC.repeat)
                {
                    entity->destroy();
                }
            }
            animationC.animation.updateSprite();
        }
    }
}

void Scene_MainMenu::init()
{
    m_assets.setResourceDir("../../res");
    
    registerTexture("Start", "start.png");
    
    registerAction(sf::Keyboard::Key::M, ActionTypes::FS);
    
    m_view = m_game->getWindow().getView();
    spawnMainMenu();
}

void Scene_MainMenu::spawnMainMenu()
{
    float offsetX {m_view.getSize().x / 10.f};
    sf::Vector2f startPos {m_game->getGrid().getGridAt(m_view.getCenter()).worldPos};
    spawnButton("Start", "Start", startPos);
    
    spawnButton("Map", "Start", m_view.getCenter() + sf::Vector2f {0.f, offsetX});
    spawnButton("SaveLoad", "Start", m_view.getCenter() + sf::Vector2f{0.f, offsetX * 2.f});
}


void Scene_MainMenu::spawnButton(const std::string& name, std::string_view tex, sf::Vector2f worldPos)
{
    auto button = m_entities.addEntity(name);
    button->add<cSprite>(m_assets.getTexture(tex));
    button->add<cTransform>(worldPos);
    button->get<cTransform>().scale = {3.f, 3.f};
    
    const auto& dimensions = button->get<cSprite>().sprite.getGlobalBounds().size;
    button->get<cSprite>().sprite.setOrigin({dimensions.x / 2.f, dimensions.y / 2.f});
}