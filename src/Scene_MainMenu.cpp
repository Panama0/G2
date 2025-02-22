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
        case ActionTypes::printa:
            if(action.status() == Action::end) { std::cout << "A!\n"; };
            break;
    }
}

void Scene_MainMenu::sRender()
{
    m_game->getWindow().clear();
    for(auto& entity : m_entities.getEntities())
    {
        if(entity->has<cSprite>())
        {
            m_game->getWindow().draw(entity->get<cSprite>().sprite);
        }
    }
    m_game->getWindow().display();
}

void Scene_MainMenu::sAnimation()
{
    for(auto& entity : m_entities.getEntities())
    {
        if(entity->has<cAnimation>() && entity->has<cSprite>())
        {
            entity->get<cAnimation>().animation.updateSprite();
        }
    }
}

void Scene_MainMenu::init()
{
    registerAsset(AssetType::font, "Menu Font", "../../res/DroidSans.ttf");
    registerAsset(AssetType::texture, "Start", "../../res/start.png");
    
    registerAsset(AssetType::animation, "TestAnimation", "../../res/animation test4.png", 4, 60);
    
    registerAction(sf::Keyboard::Key::A, ActionTypes::printa);
    
    spawnMainMenu();
}

void Scene_MainMenu::spawnMainMenu()
{
    spawnButton(sf::FloatRect{{0.f, 0.f}, {0.f, 0.f}}, "std::string text");
    
    auto animationTest = m_entities.addEntity("Animation");
    animationTest->add<cAnimation>(m_assets.getAnimation("TestAnimation"), true);
    animationTest->add<cTransform>(sf::Vector2f {300, 300});
    animationTest->add<cSprite>();
    animationTest->get<cAnimation>().animation.addSprite(&animationTest->get<cSprite>().sprite);
}


void Scene_MainMenu::spawnButton(sf::FloatRect rect, std::string text)
{
    auto button = m_entities.addEntity("Start");
    button->add<cSprite>(m_assets.getTexture("Start"));
    button->add<cTransform>(sf::Vector2f{0, 20});
    button->add<cBoundingBox>();
    
    const auto& dimensions = button->get<cSprite>().sprite.getGlobalBounds().size;
    button->get<cSprite>().sprite.setOrigin({dimensions.x / 2.f, dimensions.y / 2.f});
    
    button->get<cTransform>().pos = {300, 400};
}