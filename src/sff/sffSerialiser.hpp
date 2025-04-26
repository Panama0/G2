#pragma once

#include "sff/sffFileManipulator.hpp"
#include "sff/sffNodeData.hpp"

namespace sff
{
class Serialiser : public FileManipulator
{
public:
    void startFile(const std::string& rootTag);
    void startNode(const std::string& tag);
    void endNode();

    void writeNode(Node* node);
    void indent(uint32_t depth);

    // Add data to the current node
    void addData(const std::string& key, const NodeData& data);

    bool endFile();
private:

};
}
