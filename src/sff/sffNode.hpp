#pragma once

#include "sff/sffNodeData.hpp"

#include <cassert>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace sff
{

class Node
{
public:
    Node() = default;
    Node(std::string_view tag) : m_tag{tag} {}

    void addData(const std::string& key, const NodeData& value)
    {
        if(m_data.find(key) == m_data.end())
        {
            m_data.emplace(key, std::vector<NodeData>({value}));
        }
        else // the key exists, so add the data
        {
            m_data.at(key).push_back(value);
        }
    }

    Node* addChild(std::unique_ptr<Node> node)
    {
        m_children.push_back(std::move(node));
        return m_children.back().get();
    }

    std::string_view getTag() { return m_tag; }

    const std::vector<std::unique_ptr<Node>>& getChildren()
    {
        return m_children;
    }

    std::optional<Node*> getChild(std::string_view tag)
    {
        for(auto& child : m_children)
        {
            if(child->m_tag == tag)
            {
                return child.get();
            }
        }
        return std::nullopt;
    }

    std::optional<std::vector<NodeData>> getData(const std::string& key)
    {
        if(m_data.find(key) != m_data.end())
        {
            return m_data.at(key);
        }
        // we have not retrieved anything
        return std::nullopt;
    }

private:
    std::string m_tag;
    std::vector<std::unique_ptr<Node>> m_children;

    // key value pairs
    std::map<std::string, std::vector<NodeData>> m_data;
};
}
