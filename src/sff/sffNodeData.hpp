#pragma once

#include <cstdio>
#include <iostream>
#include <string>
#include <utility>
#include <variant>

namespace sff
{
using DataVariant = std::variant<int,
                                 float,
                                 bool,
                                 std::string,
                                 std::pair<int, int>,
                                 std::pair<float, float>>;
class NodeData
{
public:
    NodeData() = default;

    template <typename T> NodeData(const T& val) : m_data{val} {}

    // this is odd...
    template <typename T> operator T() const
    {
        const T* val = std::get_if<T>(&m_data);

        if(!val)
        {
            std::cerr << "There is nothing of this type stored!\n";
            return T{};
        }

        return *val;
    }

    std::string getString() { return std::visit(Visitor(), m_data); }
private:
    struct Visitor
    {
        std::string operator()(int i) { return std::to_string(i); }
        std::string operator()(float f) { return std::to_string(f); }
        std::string operator()(bool b) { return b ? "true" : "false"; }
        std::string operator()(std::string s) { return s; }
        std::string operator()(std::pair<int, int> vec2i)
        {
            char b[256];
            std::snprintf(b, 256, "(%d, %d)", vec2i.first, vec2i.second);
            return b;
        }
        std::string operator()(std::pair<float, float> vec2f)
        {
            char b[256];
            std::snprintf(b, 256, "(%f, %f)", vec2f.first, vec2f.second);
            return b;
        }
    };

    DataVariant m_data;
};

}
