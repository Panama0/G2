#pragma once

#include <cassert>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace sff
{
using NodeData = std::variant<std::vector<int>,
                              std::vector<bool>,
                              std::vector<std::string>,
                              std::vector<std::pair<int, int> >,
                              std::vector<std::pair<float, float> > >;

class Node
{
public:
    Node() = default;
    Node(std::string_view tag) : m_tag{tag} {}

    template <typename T> void addData(const std::string& key, const T& value)
    {
        if(m_data.find(key) == m_data.end())
        {
            m_data.emplace(key, std::vector<T>{value});
        }
        else
        {
            std::vector<T>* valueStorage
                = std::get_if<std::vector<T> >(&m_data[key]);

            if(valueStorage)
            {
                valueStorage->push_back(value);
            }
            else
            {
                assert(!"No storage for this value!\n");
            }
        }
    }

    Node* addChild(std::unique_ptr<Node> node)
    {
        m_children.push_back(std::move(node));
        return m_children.back().get();
    }

    std::string_view getTag() { return m_tag; }

    const std::vector<std::unique_ptr<Node> >& getChildren()
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

    template <typename T>
    std::optional<std::vector<T> > getData(const std::string& key)
    {
        if(m_data.find(key) != m_data.end())
        {
            auto data = std::get_if<T>(&m_data[key]);
            if(data)
            {
                return *data;
            }
        }
        // we have not retrieved anything
        return std::nullopt;
    }

private:
    std::string m_tag;
    std::vector<std::unique_ptr<Node> > m_children;

    // key value pairs
    std::map<std::string, NodeData> m_data;
};
}
