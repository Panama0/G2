#include "sff/sffParser.hpp"
#include "sff/sffNode.hpp"

#include <cctype>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <string_view>
#include <type_traits>

namespace sff
{

bool Parser::open(const std::filesystem::path& path)
{
    m_file.open(path);

    if(path.extension() != ".sff")
    {
        std::cerr
            << "Could not open file: invalid file extension. Expect .sff";
        return false;
    }

    return dead();
}

void Parser::tokenise()
{
    std::string token;
    while(m_file)
    {
        if(m_file >> token)
        {
            m_tokens.push_back(token);
        }
    }
}

std::unique_ptr<Node> Parser::parse(const std::string& rootTag)
{
    tokenise();

    m_root = std::make_unique<Node>(rootTag);
    m_currentNode = m_root.get();

    class State
    {
    public:
        enum StateTypes
        {
            root,
            node,
            data,
            dataList
        };

        StateTypes get() { return m_state.top(); }
        void push(StateTypes state) { m_state.push(state); }
        void pop() { m_state.pop(); }

        std::string currentKey;
        std::vector<std::string> currentList;

    private:
        std::stack<StateTypes> m_state;
    };

    State state;
    state.push(State::root);

    while(!dead())
    {
        if(state.get() == State::root) // look for a node
        {
            if(getToken(1) == "{")
            {
                auto tag = getToken();
                state.push(State::node);
                advance(2);

                if(tag)
                {
                    addNode(*tag);
                }
            }
        }
        else if(state.get() == State::node) // look for data
        {
            auto token = getToken();
            if(getToken(1) == "=" && getToken(2) == "{")
            {
                auto key = getToken();
                advance(3);
                state.push(State::dataList);

                if(key)
                {
                    state.currentKey = *key;
                }
            }
            else if(getToken(1) == "=")
            {
                auto key = getToken();
                state.push(State::data);
                advance(2);

                if(key)
                {
                    state.currentKey = *key;
                }
            }
            else if(getToken() == "}") // if we hit a } we exit the node
            {
                state.pop();
                advance();
            }
        }
        else if(state.get() == State::data) // get the value
        {
            auto value = getToken();
            advance();

            if(value)
            {
                if(value->front() == '(' && value->back() == ',')
                { // it is a vector2 so get the second token
                    auto token2 = getToken();
                    advance();
                    if(value && token2)
                    {
                        value->append(*token2);
                    }
                }

                auto data = parseDataValue(*value);
                m_currentNode->addData(state.currentKey, *value);
                state.pop();
            }
        }
        else if(state.get() == State::dataList) // get the values in the list
        {
            auto token = getToken();
            advance();

            // if we hit a } we exit the list
            if(token == "}")
            {
                state.pop();
            }
            else
            {
                state.currentList.push_back(*token);
                m_currentNode->addData(state.currentKey, *token);
            }
        }
    }
    return std::move(m_root);
}

std::optional<NodeData> Parser::parseDataValue(const std::string& value)
{
    if(value == "true")
    {
        return true;
    }
    else if(value == "false")
    {
        return false;
    }
    else if(value.front() == '\"' && value.back() == '\"') // it is a string
    {
        auto str = value.substr(1, value.size() - 2);
        return str;
    }
    else if(isInteger(value))
    {
        return std::stoi(value);
    }
    else if(isFloat(value))
    {
        return std::stof(value);
    }
    else if(isVec2i(value))
    {
    }
    else if(isVec2f(value))
    {
    }
    else
    {
        std::cerr << "Data type not recognized!\n";
    }

    return std::nullopt;
}

bool Parser::isInteger(std::string_view string)
{
    int64_t total{};
    if(string.at(0) == '-')
    {
        string.remove_prefix(1);
    }

    for(char c : string)
    {
        if(!std::isdigit(c))
        {
            return false;
        }

        total *= 10;
        total -= c - '0';

        if(total > INT_MAX || total > (INT64_MAX - 10))
        {
            std::cerr << "Int value is too large!\n";
            return false;
        }
    }
    return true;
}

bool Parser::isFloat(std::string_view string)
{
    uint32_t totalInt{};
    uint32_t totalDecimal{};

    if(string.at(0) == '-')
    {
        string.remove_prefix(1);
    }

    bool hitDecimalPoint{false};

    for(char c : string)
    {
        if(c == '.')
        {
            hitDecimalPoint = true;
            continue;
        }
        else if(!std::isdigit(c))
        {
            return false;
        }

        if(!hitDecimalPoint)
        {
            totalInt *= 10;
            totalInt += c - '0';
        }
        else
        {
            totalDecimal *= 10;
            totalDecimal += c - '0';
        }
    }

    if(!hitDecimalPoint)
    {
        return false;
    }

    double total = totalInt + totalDecimal / 10.0;
    if(total > FLT_MAX || total < FLT_MIN)
    {
        std::cerr << "Float value is too large\n";
        return false;
    }

    return true;
}

bool Parser::isVec2i(std::string_view value)
{
    if(value.front() != '(' && value.back() != ')')
    {
        return false;
    }
    else
    {
        value.remove_prefix(1);
        value.remove_suffix(1);
    }

    size_t mid = value.find(',');
    if(mid == value.npos)
    {
        return false;
    }
    else if(!(isInteger(value.substr(0, mid))
              && isInteger(value.substr(mid + 1, value.size()))))
    {
        return false;
    }

    return true;
}

bool Parser::isVec2f(std::string_view string) { return false;}

void Parser::addNode(const std::string& tag)
{
    auto child = m_currentNode->addChild(std::make_unique<Node>(tag));
    m_currentNode = child;
}

std::optional<std::string> Parser::getToken(uint32_t ahead)
{
    size_t peekIndex{m_offset + ahead};
    if(peekIndex < m_tokens.size())
    {
        return m_tokens[peekIndex];
    }
    else
    {
        return std::nullopt;
    }
}

void Parser::advance(uint32_t ahead)
{
    m_offset += ahead;
    if(m_offset >= m_tokens.size())
    {
        m_eof = true;
    }
}

}
