/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#include "Solver.hpp"

#include "Util.hpp"
#include "Movement.hpp"
#include "Board.hpp"
#include "Square.hpp"
#include "Pos.hpp"

#include <vector>
#include <cassert>
#include <cmath>

namespace Solver
{

Solution::Solution(Status status, std::vector<Movement::Dir> dirs,
                   std::vector<Square> squares, int depth) :
    _status(status),
    _dirs(std::move(dirs)),
    _squares(std::move(squares)),
    _depth(depth)
{
}

bool Solution::operator==(const Solution& solution) const
{
    if ((_status != solution._status) ||
        (_dirs != solution._dirs) ||
        (_depth != solution._depth)) {
        return false;
    }

    // Process this last as this is significantly costlier
    for (int dep = 0; dep < _depth; dep++) {
        if ((_squares.at(dep) == solution._squares.at(dep)) ==
            Square::Equality::NOT_EQUAL)
            return false;
    }

    return true;
}

Solution::Status Solution::GetStatus() const
{
    return _status;
}

const std::vector<Movement::Dir>& Solution::GetDir() const
{
    return _dirs;
}

const std::vector<Square>& Solution::GetSquare() const
{
    return _squares;
}

int Solution::GetDepth() const
{
    return _depth;
}

class Node
{
public:

    Node() = default;
    Node(Movement::Dir dir, Square square, bool solved = false,
         int prevIdx = -1, int depth = 0, int repeatedDepth = 0);
    Node(const Node& node) = default;
    Node(Node&& node) noexcept = default;
    ~Node() = default;

    Node& operator=(const Node& node) = default;
    Node& operator=(Node&& node) noexcept = default;

    Movement::Dir GetDir() const;
    Square& GetSquare();
    bool GetSolved() const;
    int GetPrevNode() const;
    int GetDepth() const;
    int GetRepeatedDepth() const;
    void SetRepeatedDepth(int depth);

private:

    Movement::Dir _dir{ Movement::Dir::NONE };
    Square _square{ };
    bool _solved{ false };
    int _prevNode{ -1 };
    int _depth{ 0 };
    int _repeatedDepth{ 0 };
};

Node::Node(Movement::Dir dir, Square square, bool solved,
           int prevNode, int depth, int repeatedDepth) :
    _dir(dir),
    _square(std::move(square)),
    _solved(solved),
    _prevNode(prevNode),
    _depth(depth),
    _repeatedDepth(repeatedDepth)
{
}

Movement::Dir Node::GetDir() const
{
    return _dir;
}

Square& Node::GetSquare()
{
    return _square;
}

bool Node::GetSolved() const
{
    return _solved;
}

int Node::GetPrevNode() const
{
    return _prevNode;
}

int Node::GetDepth() const
{
    return _depth;
}

int Node::GetRepeatedDepth() const
{
    return _repeatedDepth;
}

void Node::SetRepeatedDepth(int depth)
{
    _repeatedDepth = depth;
}

// Use Breadth First Search to search for shortest possible solution.  The puzzle
// is only deemed solvable if there is only a single shortest solution possible.
// The puzzle is not solvable if there is another shortest solution with a different
// Square position, or similiar Square position but different Movement::Dir set.
[[nodiscard]] Solution Solve(const Board& board, const Square& square, int maxDepth)
{
    // Precondition check
    assert(Util::IsBoardSquareSane(board, square));
    assert(!square.IsSolved());
    assert(maxDepth > 0);

    // Use Geometric Series to calculate maximum number of required nodes:
    //
    //      r * (1 - r^n) / (1 - r)
    //
    // where
    //     r == Square::NUM
    //     n == maxDepth + 1 (extra 1 for starting node)
    std::vector<Node> nodes;
    nodes.reserve(static_cast<long long>(
                  Square::NUM * (1 - std::pow(Square::NUM, maxDepth + 1)) /
                  (1 - Square::NUM)));

    int nodeCount = 0;
    Solution::Status solStatus = Solution::Status::NONE;
    size_t solNode = 0;
    int solDepth = 0;

    nodes.emplace_back(Node(Movement::Dir::NONE, square, false, -1, 0, 0));

    while (nodeCount != nodes.size()) {

        if (nodes.at(nodeCount).GetSolved()) {
            nodeCount++;
            continue;
        }

        const Square& oldSquare = nodes.at(nodeCount).GetSquare();
        int oldDepth = nodes.at(nodeCount).GetDepth();
        int newDepth = oldDepth + 1;

        // Ensure that all nodes with the same depth of solution found has been searched.
        // This is so we can detect the condition where there is another solution with
        // the same depth as the solution depth.
        if (solStatus == Solution::Status::SOLVED && newDepth > solDepth)
            break;

        if (newDepth > maxDepth) {
            solStatus = Solution::Status::MAX_DEPTH_REACHED;
            break;
        }

        for (Movement::Dir dir = Movement::Dir::UP;
             dir <= Movement::Dir::RIGHT;
             dir = static_cast<Movement::Dir>(static_cast<int>(dir) + 1)) {

            Movement::Result moveRes = Movement::Move(board, oldSquare, dir);
            if (!moveRes.IsSuccess())
                continue;

            const Square& newSquare = moveRes.GetSquare();
            bool newSolved = newSquare.IsSolved();

            bool repeated = false;

            for (auto& node : nodes) {

                if ((newSquare == node.GetSquare()) == Square::Equality::NOT_EQUAL)
                    continue;

                // Only keep track of smallest repeated depth
                if (newDepth < node.GetRepeatedDepth())
                    node.SetRepeatedDepth(newDepth);

                repeated = true;
                break;
            }

            if (!repeated)
                nodes.emplace_back(Node(dir, newSquare, newSolved, nodeCount, newDepth));

            if (newSolved) {

                if (solStatus == Solution::Status::NONE) {
                    solStatus = Solution::Status::SOLVED;
                    solNode = nodes.size() - 1;
                    solDepth = newDepth;

                } else if (newDepth == solDepth) {
                    solStatus = Solution::Status::SHORTEST_SOLUTION_REPEATED;
                    break;
                }
            }
        }

        if (solStatus == Solution::Status::SHORTEST_SOLUTION_REPEATED)
            break;

        nodeCount++;
    }

    if (solStatus != Solution::Status::SOLVED) {
        if (solStatus == Solution::Status::NONE)
            return Solution(Solution::Status::UNSOLVABLE);
        else
            return Solution(solStatus);
    }

    // Construct solution

    std::vector<Movement::Dir> retDir;
    retDir.reserve(solDepth);

    std::vector<Square> retSquare;
    retSquare.reserve(solDepth);

    size_t prevNode = solNode;

    do {
        Node& n = nodes.at(prevNode);

        if (n.GetRepeatedDepth() > 0 && n.GetRepeatedDepth() <= n.GetDepth())
            return Solution(Solution::Status::SHORTEST_SOLUTION_REPEATED);

        retDir.emplace_back(n.GetDir());
        // Safe to move as nodes are no longer needed beyond this point
        retSquare.emplace_back(std::move(n.GetSquare()));

        prevNode = n.GetPrevNode();

    } while (prevNode != -1);

    std::reverse(retDir.begin(), retDir.end());
    std::reverse(retSquare.begin(), retSquare.end());

    return Solution(solStatus, std::move(retDir), std::move(retSquare), solDepth);
}

} // namespace Solver
