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

void FileManipulator::addChild(const std::string& tag)
{
    if(!m_currentNode)
    {
        std::cerr << "No node to add to!\n";
        return;
    }
    auto child
        = m_currentNode->addChild(std::make_unique<Node>(tag, m_currentNode));
    m_currentNode = child;
}

void FileManipulator::addNode(const std::string& tag)
{
    if(!m_currentNode)
    {
        std::cerr << "No node to add to!\n";
        return;
    }

    auto parent = m_currentNode->getParent();
    if(!parent)
    {
        parent = m_root.get();
    }

    auto child = parent->addChild(std::make_unique<Node>(tag, parent));
    m_currentNode = child;
}

}
