#pragma once

#include "sff/sffFileManipulator.hpp"
#include "sff/sffNodeData.hpp"
#include <cstdint>

namespace sff
{
class Serialiser : public FileManipulator
{
public:
    // Add data to the current node
    void addData(const std::string& key, const NodeData& data);

    // Writes the file to the disk
    bool endFile();
private:
    void writeNode(Node* node, uint32_t depth);
    void indent(uint32_t depth);
};
}
