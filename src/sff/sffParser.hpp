#pragma once

#include "sff/sffNode.hpp"

#include <fstream>
#include <filesystem>
#include <memory>

namespace sff
{
class Parser
{
public:
    bool open(const std::filesystem::path& path);
    
    Node* parse();
    
    bool alive() { return m_file.good(); }
    
    void close();

private:
    std::unique_ptr<Node> m_root;
    Node* currentNode;
    std::fstream m_file;
};
}