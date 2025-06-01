#include "GameEngine.hpp"
#include "scenes/Scene_Editor.hpp"

int main()
{
        GameEngine g;
        g.startScene(std::make_unique<Scene_Editor>(&g));
        g.run();
}
