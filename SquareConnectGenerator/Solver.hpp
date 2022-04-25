/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "Movement.hpp"
#include "Square.hpp"

#include <vector>

namespace Solver
{

class Solution
{
public:

    enum class Status : int
    {
        NONE = 0,
        SOLVED,
        UNSOLVABLE,
        MAX_DEPTH_REACHED,
        SHORTEST_SOLUTION_REPEATED,
    };

    Solution() = default;
    Solution(Status status, std::vector<Movement::Dir> dirs = { },
             std::vector<Square> squares = { }, int depth = 0);
    Solution(const Solution& solution) = default;
    Solution(Solution&& solution) noexcept = default;

    ~Solution() = default;

    Solution& operator=(const Solution& solution) = default;
    Solution& operator=(Solution&& solution) noexcept = default;

    bool operator==(const Solution& solution) const;

    Status GetStatus() const;
    const std::vector<Movement::Dir>& GetDir() const;
    const std::vector<Square>& GetSquare() const;
    int GetDepth() const;

private:

    Status _status{ Status::NONE };
    std::vector<Movement::Dir> _dirs{ };
    std::vector<Square> _squares{ };
    int _depth{ 0 };
};

[[nodiscard]] Solution Solve(const Board& board, const Square& square, int maxDepth);

};


#endif // SOLVER_HPP
