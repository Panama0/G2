#pragma once

#include "SFML/System/Vector2.hpp"

#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>

struct Node;

struct CompareNodes;

struct VecHash;

using NodeList = std::unordered_map<sf::Vector2i, std::unique_ptr<Node>, VecHash>;

// Heuristics
float diagonal(const sf::Vector2i& start, const sf::Vector2i& end);

Node* getNode(const sf::Vector2i& pos, NodeList& nodes);

std::vector<sf::Vector2i> findPath(
    const sf::Vector2i& start,
    const sf::Vector2i& end,
    const std::function<bool(const sf::Vector2i& pos)>& pathable,
    const std::function<float(const sf::Vector2i& pos)>& getWeight);
