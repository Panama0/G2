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
    
    virtual void update();
    virtual void sDoAction(const Action& action);
    virtual void sRender();
    virtual void init();
private:
    void spawnMainMenu();
    void spawnButton(sf::FloatRect rect, std::string text, const Action& action);
};