#pragma once

#include "sff/sffFileManipulator.hpp"
#include "sff/sffNodeData.hpp"
#include <cstdint>
#include <filesystem>

namespace sff
{
// Class for writing to a file
class Serialiser : public FileManipulator
{
public:
    Serialiser() = default;
    Serialiser(const std::filesystem::path& path)
        : FileManipulator(path, std::ios::out)
    {
    }
    // Add data to the current node
    void addData(const std::string& key, const NodeData& data);

    // Writes the file to the disk
    bool endFile();

private:
    void writeNode(Node* node, uint32_t depth);
    void indent(uint32_t depth);
};
}
