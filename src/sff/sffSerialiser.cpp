#include "sff/sffSerialiser.hpp"
#include <cassert>

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

    //WARN: maybe in the future we want to have this happen in addNode()?
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

void Serialiser::addData(const std::string& key,
                                    const NodeData& data)
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

bool Serialiser::write()
{
    assert(m_nodeStack.size() == 1 && "Node stack has too many or too little nodes!\n");

    if(!alive())
    {
        return false;
    }

    m_nodeStack.push(m_root.get());

    while(!m_nodeStack.empty())
    {
        const auto& children = m_nodeStack.top()->getChildren();

        std::cout << m_nodeStack.top()->getTag() << std::endl;

        m_nodeStack.pop();
        for(const auto& child : children)
        {
            m_nodeStack.push(child.get());
        }
    }

    return true;
}

}
