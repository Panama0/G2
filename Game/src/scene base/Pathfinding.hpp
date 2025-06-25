#pragma once

#include "Vec2.hpp"

#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>

struct Node;

struct CompareNodes;

struct VecHash;

using NodeList = std::unordered_map<Vec2i, std::unique_ptr<Node>, VecHash>;

// Heuristics
float diagonal(const Vec2i& start, const Vec2i& end);

Node* getNode(const Vec2i& pos, NodeList& nodes);

std::vector<Vec2i> findPath(
    const Vec2i& start,
    const Vec2i& end,
    const std::function<bool(const Vec2i& pos)>& pathable,
    const std::function<float(const Vec2i& pos)>& getWeight);
