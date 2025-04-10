#pragma once

#include "sff/sffNode.hpp"

#include <fstream>
#include <filesystem>
#include <memory>
#include <optional>

namespace sff
{
class Parser
{
public:
    bool open(const std::filesystem::path& path);
    
    std::unique_ptr<Node> parse(const std::string& rootTag);
    
    bool alive() { return m_file.good(); }
    
    void close();

private:
    std::optional<std::string> peekToken(uint32_t ahead = 1);
    std::optional<char> peek(uint32_t ahead = 1);
    void consumeToken(uint32_t ahead = 1);
    void consume(uint32_t ahead = 1);

    std::unique_ptr<Node> m_root;
    Node* m_currentNode;
    std::fstream m_file;
};
}