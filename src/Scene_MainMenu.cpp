#include "Scene_MainMenu.hpp"

#include <iostream>

void Scene_MainMenu::update()
{
    m_entities.update();
    
    for(auto& entity : m_entities.getEntities())
    {
        if(entity->has<cTransform>() && entity->has<cSprite>())
        {
            auto& spr = entity->get<cSprite>().sprite;
            const auto& pos = entity->get<cTransform>().pos;
            spr.setPosition(pos);
        }
    }
    sRender();
}

void Scene_MainMenu::sDoAction(const Action& action)
{
    if(action.name() == "PrintA" && action.status() == Action::end)
    {
        std::cout << "A!!!\n";
    }
}

void Scene_MainMenu::sRender()
{
    m_game->getWindow().clear(sf::Color{sf::Color::Red});
    for(auto& entity : m_entities.getEntities())
    {
        if(entity->has<cSprite>())
        {
            m_game->getWindow().draw(entity->get<cSprite>().sprite);
        }
    }
    m_game->getWindow().display();
}

void Scene_MainMenu::init()
{
    registerAsset(AssetType::font, "Menu Font", "../../res/DroidSans.ttf");
    registerAsset(AssetType::texture, "Button1", "../../res/button.png");
    
    registerAction(sf::Keyboard::Key::A, "PrintA");
    
    spawnMainMenu();
}

void Scene_MainMenu::spawnMainMenu()
{

}


void Scene_MainMenu::spawnButton(sf::FloatRect rect, std::string text, const Action& action)
{
    auto button = m_entities.addEntity("Button1");
    button->add<cSprite>(m_assets.getTexture("Button1"));
    button->add<cTransform>(sf::Vector2f{0, 20});
    button->add<cBoundingBox>();
}