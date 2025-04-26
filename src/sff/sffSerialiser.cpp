#include "sff/sffSerialiser.hpp"
#include <cassert>
#include <cstdint>
#include <ostream>
#include <string_view>
#include <sys/types.h>

namespace sff
{

void Serialiser::startFile(const std::string& rootTag)
{
    m_root = std::make_unique<Node>(rootTag, nullptr);
    m_nodeStack.emplace(m_root.get());
}

void Serialiser::startNode(const std::string& tag)
{
    auto node = addNode(tag, currentNode());

    // WARN: maybe in the future we want to have this happen in addNode()?
    m_nodeStack.emplace(node);
}

void Serialiser::endNode()
{
    if(!m_nodeStack.empty())
    {
        m_nodeStack.pop();
    }
    else
    {
        std::cerr << "No node to pop!\n";
    }
}

void Serialiser::addData(const std::string& key, const NodeData& data)
{
    if(currentNode())
    {
        currentNode()->addData(key, data);
    }
    else
    {
        std::cerr << "There is no current node!\n";
    }
}

bool Serialiser::endFile()
{
    if(!alive())
    {
        return false;
    }

    assert(m_nodeStack.size() == 1
           && "Node stack invalid! Missing an endNode()?\n");

    writeNode(m_root.get());

    return true;
}

void Serialiser::writeNode(Node* node)
{
    static uint32_t depth{};
    depth++;
    assert(depth < UINT32_MAX && "Max depth reached!\n");

    // depth local to this function call
    uint32_t localDepth = depth;

    indent(depth);

    m_file << node->getTag() << " {" << std::endl;
    // write data
    for(const auto& [key, values] : node->getData())
    {
        indent(depth + 1);
        m_file << key << " = ";

        if(values.size() == 0)
        {
            m_file << "none";
        }
        else if(values.size() == 1)
        {
            m_file << values.front().getString();
        }
        else if(values.size() > 1)
        {
            m_file << '{' << std::endl;
            for(const auto& value : values)
            {
                indent(depth + 2);
                m_file << value.getString() << std::endl;
            }
            indent(depth + 1);
            m_file << '}';
        }
        m_file << std::endl;
    }

    for(const auto& child : node->getChildren())
    {
        writeNode(child.get());
    }

    indent(localDepth);

    m_file << '}' << std::endl;
}

void Serialiser::indent(uint32_t depth)
{
    for(uint32_t i{}; i < (depth - 1) * 4; i++)
    {
        m_file << ' ';
    }
}

}
