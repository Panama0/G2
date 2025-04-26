#include "sff/sffFileManipulator.hpp"
#include <ios>
#include <memory>
#include <string>

namespace sff
{

bool FileManipulator::open(const std::filesystem::path& path,
                           std::ios_base::openmode mode)
{
    if(path.extension() != ".sff")
    {
        std::cerr
            << "Could not open file: invalid file extension. Expect .sff";
        return false;
    }
    m_file.open(path, mode);

    return m_file.is_open();
}

Node* FileManipulator::addNode(const std::string& tag, Node* parent)
{
    if(!currentNode())
    {
        std::cerr << "No node to add to!\n";
        return nullptr;
    }
    auto child = currentNode()->addChild(std::make_unique<Node>(tag, parent));

    return child;
}

Node* FileManipulator::currentNode()
{
    if(!m_nodeStack.empty())
    {
        return m_nodeStack.top();
    }
    else
    {
        return nullptr;
    }
}

}
