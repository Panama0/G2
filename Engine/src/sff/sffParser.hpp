#pragma once

#include "sff/sffFileManipulator.hpp"
#include "sff/sffNode.hpp"

#include <memory>
#include <optional>
#include <string>
#include <string_view>

namespace sff
{
// Class for reading from a file
class Parser : public FileManipulator
{
public:
    Parser() = default;
    Parser(const std::filesystem::path& path)
        : FileManipulator(path, std::ios::in)
    {
    }
    // Parse and return the currently open file
    std::unique_ptr<Node> parse();

    bool eof() override { return m_eof; }
private:
    void tokenise();
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

    std::vector<std::string> m_tokens;
    size_t m_offset{};
    bool m_eof{false};
};
}
