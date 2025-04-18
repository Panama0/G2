#pragma once

#include <string>
#include <utility>
#include <variant>

namespace sff
{
using DataVariant = std::variant<int,
                                 bool,
                                 std::string,
                                 std::pair<int, int>,
                                 std::pair<float, float>>;
class NodeData
{
public:
    NodeData() = default;

    template <typename T> NodeData(const T& val) : m_data{val} {}

    template <typename T> const T& get() { return m_data; }

    template <typename T> bool set(const T& val) { m_data = val; }

private:
    DataVariant m_data;
};

}
