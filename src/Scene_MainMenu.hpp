#include "Scene.hpp"

class Scene_MainMenu : public Scene
{
public:
    enum ActionTypes
    {
        null,
        printa,
    };
    
    Scene_MainMenu() = default;
    Scene_MainMenu(GameEngine* game)
        :Scene(game)
    { init(); }
    
    void update();
    void sDoAction(const Action& action);
    void sRender();
    void sAnimation();
    void init();
private:
    void spawnMainMenu();
    void spawnButton(sf::FloatRect rect, std::string text);
};