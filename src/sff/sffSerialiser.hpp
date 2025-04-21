#pragma once

#include "sff/sffFileManipulator.hpp"
#include "sff/sffNodeData.hpp"
#include <string_view>
#include <stack>

namespace sff
{
class Serialiser : public FileManipulator
{
public:
    // we could add overloads to add who vectors at once? or have the caller just loop though
    // void addData(std::string_view nodeName, const NodeData& data);
    void startFile(const std::string& rootTag);
    std::string_view getCurrentNode();
    void addDataCurrentNode(const std::string& key, const NodeData& data);
    bool write();
private:

    std::stack<Node*> m_nodeStack;
};
}
