#pragma once

#include "sff/sffNode.hpp"

#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace sff
{
class Parser
{
public:
    bool open(const std::filesystem::path& path);

    std::unique_ptr<Node> parse(const std::string& rootTag);

    bool dead() { return m_eof; }

    void close();

private:
    void tokenise();
    void addNode(const std::string& tag);
    std::optional<std::string> getToken(uint32_t ahead = 0);
    void advance(uint32_t ahead = 1);

    std::optional<NodeData> parseDataValue(const std::string& value);
    bool isInteger(std::string_view string);
    bool isFloat(std::string_view string);
    std::optional<std::pair<std::string_view, std::string_view>> getVector2(
        std::string_view string);
    bool isVector2f(std::string_view string);
    bool isVector2i(std::string_view string);
    std::pair<float, float> getVector2f(std::string_view string);
    std::pair<int, int> getVector2i(std::string_view string);

    std::unique_ptr<Node> m_root;
    Node* m_currentNode;
    std::fstream m_file;

    std::vector<std::string> m_tokens;
    size_t m_offset{};
    bool m_eof{false};
};
}
