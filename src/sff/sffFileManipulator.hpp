#pragma once

#include "sff/sffNode.hpp"
#include <filesystem>
#include <fstream>
#include <string>

namespace sff
{

// Base class for serialiser and parser
class FileManipulator
{
public:
    FileManipulator() = default;
    FileManipulator(const std::filesystem::path& path) : m_file{path} {}

    bool open(const std::filesystem::path& path);
    void addNode(const std::string& name);

    virtual bool dead() { return m_file.eof(); };
    void close() { m_file.close(); }

protected:
    Node* m_currentNode;
    std::unique_ptr<Node> m_root;

    std::fstream m_file;
};

}
