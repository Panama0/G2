#include "sff/sffFileManipulator.hpp"

namespace sff
{

bool FileManipulator::open(const std::filesystem::path& path)
{
    if(path.extension() != ".sff")
    {
        std::cerr
            << "Could not open file: invalid file extension. Expect .sff";
        return false;
    }
    m_file.open(path);

    return dead();
}

void FileManipulator::addNode(const std::string& tag)
{
    auto child = m_currentNode->addChild(std::make_unique<Node>(tag));
    m_currentNode = child;
}

}

