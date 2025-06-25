#pragma once

#include "scene base/Pathfinding.hpp"
#include "SFML/System/Vector2.hpp"

#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>

struct Node
{
    Node() = default;
    Node(const sf::Vector2i& _pos) : pos{_pos} {}

    sf::Vector2i pos;
    Node* parent{};
    float gCost{0.f};
    float hCost{0.f};

    bool operator>(const Node& rhs) const
    {
        return gCost + hCost > rhs.gCost + rhs.hCost;
    }

    bool operator<(const Node& rhs) const
    {
        return gCost + hCost < rhs.gCost + rhs.hCost;
    }
};

struct CompareNodes
{
    bool operator()(Node* a, Node* b) { return *a > *b; }
};

struct VecHash
{
    size_t operator()(const sf::Vector2i& vec) const
    {
        return std::hash<int>{}(vec.x + vec.y);
    }
};

// Heuristics

float diagonal(const sf::Vector2i& start, const sf::Vector2i& end)
{
    // length of each node
    float D = 1.f;
    // diagonal distance between each node
    float D2 = std::sqrt(2.f);

    float dx = std::abs(start.x - end.x);
    float dy = std::abs(start.y - end.y);

    return D * (dx + dy) + (D2 - 2 * D) * std::min(dx, dy);
}

Node* getNode(const sf::Vector2i& pos, NodeList& nodes)
{
    if(nodes.find(pos) == nodes.end())
    {
        nodes[pos] = std::make_unique<Node>(pos);
    }
    return nodes[pos].get();
}

bool isDiagonal(const sf::Vector2i& a, const sf::Vector2i& b)
{
    int dx = std::abs(a.x - b.x);
    int dy = std::abs(a.y - b.y);
    return dx == 1 && dy == 1;
}

std::vector<sf::Vector2i> printPath(Node* node) {}

std::vector<sf::Vector2i> findPath(
    const sf::Vector2i& start,
    const sf::Vector2i& end,
    const std::function<bool(const sf::Vector2i& pos)>& pathable,
    const std::function<float(const sf::Vector2i& pos)>& getWeight)
{
    std::priority_queue<Node*, std::vector<Node*>, CompareNodes> openList;

    std::unordered_set<Node*> closedSet;

    // owns the nodes
    NodeList nodeList;

    openList.push(getNode(start, nodeList));

    constexpr std::array<sf::Vector2i, 8> directions = {
        sf::Vector2i{1, 0},  // right
        sf::Vector2i{-1, 0}, // left
        sf::Vector2i{0, 1},  // down
        sf::Vector2i{0, -1}, // up
        sf::Vector2i{1, 1},  // down-right
        sf::Vector2i{-1, 1}, // down-left
        sf::Vector2i{1, -1}, // up-right
        sf::Vector2i{-1, -1} // up-left
    };

    while(!openList.empty())
    {
        Node* currentNode{openList.top()};
        openList.pop();

        if(currentNode->pos == end)
        {
            // we found the end
            std::vector<sf::Vector2i> path;
            Node* current{currentNode};

            while(current)
            {
                path.push_back(current->pos);
                current = current->parent;
            }
            return path;
        }

        closedSet.emplace(currentNode);

        // explore surrounding tiles
        for(const auto& direction : directions)
        {
            auto newPos = currentNode->pos + direction;
            auto newNode = getNode(newPos, nodeList);

            if(newNode && pathable(newNode->pos))
            {
                // moving diagonal costs more
                auto moveCost = isDiagonal(currentNode->pos, newNode->pos)
                                    ? std::sqrt(2.f)
                                    : 1.f;

                auto newGCost
                    = currentNode->gCost + getWeight(newNode->pos) * moveCost;

                if(newGCost < newNode->gCost || newNode->gCost == 0.f)
                {
                    newNode->gCost = newGCost;
                    newNode->hCost = diagonal(newNode->pos, end);

                    newNode->parent = currentNode;

                    // must only push if the new node is better
                    openList.push(newNode);
                }
            }
        }
    }

    // no valid path found after all nodes explored
    return {};
}
