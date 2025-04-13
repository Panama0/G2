#include "sff/sffParser.hpp"
#include "sff/sffNode.hpp"

#include <string>
#include <memory>
#include <filesystem>
#include <iostream>
#include <optional>

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

    while(alive())
    {
        // check for a new node - Name {
        // check for data - Name =
        consumeToken(2);
        
        std::string test;
        m_file >> test;
        
    }
    
    return std::move(m_root);
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
    int token {};
    for(uint32_t i {}; i < ahead; i++)
    {
        while(token != std::istream::traits_type::eof() &&
            token != ' ' &&
            token != '\n')
        {
            token = m_file.get();
        }
        // skip the whitespace
        m_file.ignore();
        while(m_file.peek() == ' ' || m_file.peek() == '\n')
        {
            token = m_file.get();
        }
    }
}

void Parser::consume(uint32_t ahead)
{
    m_file.ignore(ahead);
}

}
