#pragma once

#include "sff/sffNode.hpp"
#include <filesystem>
#include <fstream>
#include <ios>
#include <string>

namespace sff
{

// Base class for serialiser and parser
class FileManipulator
{
public:
    FileManipulator() = default;
    FileManipulator(const std::filesystem::path& path) : m_file{path} {}

    bool open(const std::filesystem::path& path, std::ios_base::openmode mode);

    // adds a child to the current child
    void addChild(const std::string& name);
    // adds a child on the same level as the current child
    void addNode(const std::string& name);

    virtual bool eof() { return m_file.eof(); };
    bool alive() { return m_file.good(); }
    void close() { m_file.close(); }

protected:
    Node* m_currentNode;
    std::unique_ptr<Node> m_root;

    std::fstream m_file;
};

}
