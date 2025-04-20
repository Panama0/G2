#pragma once

#include <string>
#include <utility>
#include <variant>
#include <iostream>

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
    template <typename T>
    operator T() const
    {
        const T* val = std::get_if<T>(&m_data);

        if(!val)
        {
            std::cerr << "There is nothing of this type stored!\n";
            return T{};
        }

        return *val;
    }

private:
    DataVariant m_data;
};

}
