/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#ifndef UTIL_HPP
#define UTIL_HPP

#include "Solver.hpp"
#include "Board.hpp"
#include "Square.hpp"

#include <vector>

namespace Util
{

[[nodiscard]] int GetRandomInt(int min, int max);

[[nodiscard]] bool IsSquareWithinBoard(const Square& square);
[[nodiscard]] bool IsBoardSquareSane(const Board& board, const Square& square);
[[nodiscard]] bool IsBoardSquareSolutionSane(const Board& board, const Square& square,
                                             const Solver::Solution& solution);

// Char map is as follows:
//     'O' => empty
//     'X' => wall
//     'A' => square0
//     'B' => square1
//     'C' => square2
//     'D' => square3
void CharVectorToBoardAndSquare(const std::vector<char>& tiles,
                                Board& board, Square& square);
void BoardAndSquareToCharVector(const Board& board, const Square& square,
                                std::vector<char>& charVec);

} // namespace Util

#endif // UTIL_HPP
