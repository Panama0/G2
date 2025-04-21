#include "sff/sffNodeData.hpp"
#include "sff/sffParser.hpp"
#include "sff/sffSerialiser.hpp"
#include <utility>

int main()
{
    // GameEngine g;
    // g.run();

    // sff::Parser p;
    // p.open("../../../res/saves/test.sff");
    // auto test = p.parse("b");
    // p.close();

    sff::Serialiser s;
    bool open = s.open("../../../res/saves/testWrite.sff", std::ios_base::out);
    s.startFile("root");
    s.addNode("depth1");
    s.addDataCurrentNode("testStr", "string");
    s.addDataCurrentNode("testBool", true); 
    s.addDataCurrentNode("testFloat", 5.2f); 
    s.addDataCurrentNode("testVec2i", std::pair<int, int>{1, 55}); 
    s.addNode("depth1a");
    s.addChild("depth2 - child of depth1a");
    s.addNode("depth2a - child of depth1a");
    bool write = s.write();
    s.close();

}
