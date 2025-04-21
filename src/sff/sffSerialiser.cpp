#include "sff/sffSerialiser.hpp"

namespace sff
{

// void Serialiser::addData(std::string_view nodeName, const NodeData& data) {}
void Serialiser::startFile(const std::string& rootTag)
{
    m_root = std::make_unique<Node>(rootTag, nullptr);
    m_currentNode = m_root.get();
}

std::string_view Serialiser::getCurrentNode()
{
    if(m_currentNode)
    {
        return m_currentNode->getTag();
    }
    else
    {
        std::cerr << "No current node!\n";
        return {};
    }
}
void Serialiser::addDataCurrentNode(const std::string& key,
                                    const NodeData& data)
{
    if(m_currentNode)
    {
        m_currentNode->addData(key, data);
    }
    else
    {
        std::cerr << "There is no current node!\n";
    }
}

bool Serialiser::write()
{
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
