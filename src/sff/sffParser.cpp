#include "sff/sffParser.hpp"
#include "sff/sffNode.hpp"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <stack>
#include <string>

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
                        m_currentNode->addData(state.currentKey, *value);
                        state.pop();
                    }
                }
                else
                {
                    auto data = parseDataValue(*value);
                    m_currentNode->addData(state.currentKey, *value);
                    state.pop();
                }
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
                // state.currentList.push_back(*token);
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
        return {true};
    }
    else if(value == "false")
    {
        return {false};
    }
    else if(value.front() == '\"' && value.back() == '\"') // it is a string
    {
        auto str = value.substr(1, value.size() - 1);
    }
    else
    {
        return std::nullopt;
    }
}

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
