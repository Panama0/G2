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

    // create the root node for the user when writing to a file
    // this way it is more intuative
    if(mode == std::ios_base::out)
    {
        addNode(path.stem());
    }

    return m_file.is_open();
}

Node* FileManipulator::addNode(const std::string& tag)
{
    if(!currentNode())
    {
        // there is no current node, so make the root
        m_root = std::make_unique<Node>(tag);
        m_nodeStack.push(m_root.get());
        return m_root.get();
    }

    auto child = currentNode()->addChild(std::make_unique<Node>(tag));
    m_nodeStack.push(child);

    return child;
}

void FileManipulator::endNode()
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
