#include "engine/GameEngine.hpp"
#include "sff/sffParser.hpp"

int main()
{
    // GameEngine g;
    // g.run();

    sff::Parser p;
    p.open("../../../res/saves/test.sff");
    p.parse("b");
}

