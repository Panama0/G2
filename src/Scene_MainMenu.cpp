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
    
    // buttons
    registerTexture("Start", "start.png");
    registerTexture("Load", "load.png");
    registerTexture("Save", "save.png");
    registerTexture("Editor", "editor.png");

    registerAction(sf::Keyboard::Key::V, ActionTypes::FS);
    
    m_view = m_game->getWindow().getView();
    spawnMainMenu();
}

void Scene_MainMenu::spawnMainMenu()
{
    float offsetY {m_view.getSize().y / 10.f};
    sf::Vector2f startPos {m_game->getGrid().getGridAt(m_view.getCenter()).worldPos};
    spawnButton("Start", "Start", startPos);
    
    spawnButton("Map", "Start", m_view.getCenter() + sf::Vector2f {0.f, offsetY * 2.f});
    
    const auto& sizeSave = static_cast<sf::Vector2f>(m_assets.getTexture("Save").getSize());
    const auto& sizeLoad = static_cast<sf::Vector2f>(m_assets.getTexture("Load").getSize());
    spawnButton("Save", "Save", m_view.getCenter() + sf::Vector2f {-sizeSave.x * 3.f, offsetY * 4.f});
    spawnButton("Load", "Load", m_view.getCenter() + sf::Vector2f {sizeLoad.x * 3.f, offsetY * 4.f});
    
    const auto& sizeEditor = static_cast<sf::Vector2f> (m_assets.getTexture("Editor").getSize());
    spawnButton("Editor", "Editor", sf::Vector2f {sizeEditor.x * 2.f, offsetY * 9.f});
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