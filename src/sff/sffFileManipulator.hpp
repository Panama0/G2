#pragma once

#include "sff/sffNode.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <ios>
#include <stack>
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

    Node* currentNode();
    Node& getFile()
    {
        assert(m_root
               && "No file started! Did you forget to call startFile()?\n");
        return *m_root;
    }

    virtual bool eof() { return m_file.eof(); };
    bool alive() { return m_file.good(); }
    void close() { m_file.close(); }

protected:
    Node* addNode(const std::string& name, Node* parent);

    std::unique_ptr<Node> m_root;
    std::stack<Node*> m_nodeStack;

    std::fstream m_file;
};

}
