#include "sff/sffParser.hpp"
#include "sff/sffNode.hpp"

#include <cctype>
#include <cfloat>
#include <charconv>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace sff
{

void Parser::tokenise()
{
    std::string token;
    std::stringstream fullString;

    while(m_file)
    {
        if(m_file >> token)
        {
            // detect and concatenate multi word strings
            if(token.front() == '\"' && token.back() != '\"')
            {
                while(token.back() != '\"')
                {
                    fullString << token << ' ';
                    m_file >> token;
                }
                // get the last one
                fullString << token;
                m_tokens.push_back(fullString.str());
            }
            else // Nothing out of the ordinary, so just push the token
            {
                m_tokens.push_back(token);
            }
        }
    }
}

std::unique_ptr<Node> Parser::parse()
{
    tokenise();

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

    private:
        std::stack<StateTypes> m_state;
    };

    State state;
    state.push(State::root);

    while(!eof())
    {
        if(state.get() == State::root
           || state.get() == State::node) // look for a node
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

        if(state.get() == State::node) // look for data
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
                endNode();
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
                currentNode()->addData(state.currentKey, *data);
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
                auto data = parseDataValue(*token);
                currentNode()->addData(state.currentKey, *data);
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
    else if(isVector2i(value))
    {
        return getVector2i(value);
    }
    else if(isVector2f(value))
    {
        return getVector2f(value);
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
    if(std::abs(total) > FLT_MAX)
    {
        std::cerr << "Float value does not fit\n";
        return false;
    }

    return true;
}

std::optional<std::pair<std::string_view, std::string_view>> Parser::
    getVector2(std::string_view string)
{
    if(string.front() != '(' && string.back() != ')')
    {
        return std::nullopt;
    }
    else
    {
        string.remove_prefix(1);
        string.remove_suffix(1);
    }

    size_t mid = string.find(',');
    if(mid == string.npos)
    {
        return std::nullopt;
    }

    return std::pair{string.substr(0, mid),
                     string.substr(mid + 1, string.size())};
}

bool Parser::isVector2f(std::string_view string)
{
    auto vector2 = getVector2(string);
    if(!vector2)
    {
        // it is not a vector2
        return false;
    }

    if(!(isFloat(vector2->first) && isFloat(vector2->second)))
    {
        return false;
    }

    return true;
}

bool Parser::isVector2i(std::string_view string)
{
    auto vector2 = getVector2(string);
    if(!vector2)
    {
        // it is not a vector2
        return false;
    }

    if(!(isInteger(vector2->first) && isInteger(vector2->second)))
    {
        return false;
    }

    return true;
}

std::pair<float, float> Parser::getVector2f(std::string_view string)
{
    auto vec2 = getVector2(string).value();
    float first{};
    float second{};
    // TODO: change all to use std::from chars to avoid allocations/temp
    // strings
    auto resultFirst = std::from_chars(
        vec2.first.data(), (vec2.first.data() + vec2.first.length()), first);

    auto resultSecond
        = std::from_chars(vec2.second.data(),
                          (vec2.second.data() + vec2.second.length()),
                          second);

    return {first, second};
}

std::pair<int, int> Parser::getVector2i(std::string_view string)
{
    auto vec2 = getVector2(string).value();
    int first{};
    int second{};
    // TODO: change all to use std::from chars to avoid allocations/temp
    // strings
    auto resultFirst = std::from_chars(
        vec2.first.data(), (vec2.first.data() + vec2.first.length()), first);

    auto resultSecond
        = std::from_chars(vec2.second.data(),
                          (vec2.second.data() + vec2.second.length()),
                          second);

    return {first, second};
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
