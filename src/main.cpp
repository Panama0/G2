#include "sff/sffNodeData.hpp"
#include "sff/sffParser.hpp"
#include "sff/sffSerialiser.hpp"
#include <string_view>
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

    s.startNode("depth1");
    s.addData("test", true);
    s.startNode("depth2");
    s.addData("fa", false);

    s.endNode();
    s.endNode();

    auto& test = s.getFile()["d"]["depth2"];

    bool write = s.write();
    s.close();

}
