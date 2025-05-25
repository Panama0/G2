#include "sff/sffSerialiser.hpp"
#include <cassert>
#include <cstdint>
#include <ostream>
#include <string_view>

namespace sff
{

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

    writeNode(m_root.get(), 0);

    return true;
}

void Serialiser::writeNode(Node* node, uint32_t depth)
{
    assert(depth < UINT32_MAX - 1 && "Max depth reached!\n");

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
        writeNode(child.get(), depth + 1);
    }

    indent(depth);

    m_file << '}' << std::endl;
}

void Serialiser::indent(uint32_t depth)
{
    uint32_t indentSize {4};
    for(uint32_t i{}; i < depth * indentSize; i++)
    {
        m_file << ' ';
    }
}

}
