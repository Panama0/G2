#include "sff/sffParser.hpp"
#include "sff/sffNode.hpp"

#include <cstdint>
#include <string>
#include <memory>
#include <filesystem>
#include <iostream>
#include <optional>
#include <stack>

namespace sff{

bool Parser::open(const std::filesystem::path& path)
{
    m_file.open(path);
    
    if(path.extension() != ".sff")
    {
        std::cerr << "Could not open file: invalid file extension. Expect .sff";
        return false;
    }
    
    return alive();
}

std::unique_ptr<Node> Parser::parse(const std::string& rootTag)
{
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

    private:
        std::stack<StateTypes> m_state;
    };

    State state;
    state.push(State::root);

    while(alive())
    {
        if(state.get() == State::root) // look for a node
        {
            if(peekToken(2) == "{")
            {
                auto tag = peekToken();
                state.push(State::node);
                consumeToken(2);

                if(tag)
                {
                    addNode(*tag);
                }
            }
        }
        else if(state.get() == State::node) // look for data
        {
            if(peekToken(2) == "=" && peekToken(3) == "{")
            {
                consumeToken(3);
                state.push(State::dataList);
            }
            else if(peekToken(2) == "=")
            {
                auto key = peekToken();
                state.push(State::data);
                consumeToken(2);

                if(key)
                {
                    state.currentKey = *key;
                }
            }
            else if(peekToken() == "}") // if we hit a } we exit the node
            {
                state.pop();
            }
        }
        else if(state.get() == State::data)
        {
            auto value = peekToken();
            consumeToken();
            //TODO: process the value
            if(value)
            {
                m_currentNode->addData(state.currentKey,*value);
                state.pop();
            }
            // if we hit a , we exit the data
        }
        else if(state.get() == State::dataList)
        {

            // if we hit a } we exit the list
        }
    }
    
    return std::move(m_root);
}

void Parser::addNode(const std::string& tag)
{
    auto child = m_currentNode->addChild(std::make_unique<Node>(tag));
    m_currentNode = child;
}

std::optional<std::string> Parser::peekToken(uint32_t ahead)
{
    const auto& posBefore = m_file.tellg();
    const auto& stateBefore = m_file.rdstate();
    
    std::string token;
    for(uint32_t i {}; i < ahead; i++)
    {
        m_file >> token;
    }

    if(!m_file)
    {
        m_file.clear();
        return std::nullopt;
    }
    // put back everything
    m_file.seekg(posBefore);
    m_file.setstate(stateBefore);

    return token;
}

std::optional<char> Parser::peek(uint32_t ahead)
{
    const auto& posBefore = m_file.tellg();
    const auto& stateBefore = m_file.rdstate();
    
    int result {};
    for(uint32_t i {}; i < ahead; i++)
    {
        result = m_file.get();
    }

    if(result == std::istream::traits_type::eof())
    {
        return std::nullopt;
        m_file.clear();
    }

    // put back everything
    m_file.seekg(posBefore);
    m_file.setstate(stateBefore);
    return result;
}
void Parser::consumeToken(uint32_t ahead)
{
    /*int token {};*/
    /*char tokenTest {};*/
    /*for(uint32_t i {}; i < ahead; i++)*/
    /*{*/
    /*    while(token != std::istream::traits_type::eof() &&*/
    /*        token != ' ' &&*/
    /*        token != '\n')*/
    /*    {*/
    /*        token = m_file.get();*/
    /*        tokenTest = token;*/
    /*    }*/
    /*    // skip the whitespace*/
    /*    m_file.ignore();*/
    /*    while(m_file.peek() == ' ' || m_file.peek() == '\n')*/
    /*    {*/
    /*        token = m_file.get();*/
    /*        tokenTest = token;*/
    /*    }*/
    /*}*/

    std::string token;
    for(uint32_t i {}; i < ahead; i++)
    {
        m_file >> token;
    }
}

void Parser::consume(uint32_t ahead)
{
    m_file.ignore(ahead);
}

}
