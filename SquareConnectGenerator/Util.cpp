/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#include "Util.hpp"

#include "Solver.hpp"
#include "Board.hpp"
#include "Square.hpp"
#include "Pos.hpp"

#include <array>
#include <vector>
#include <thread>
#include <random>
#include <stdexcept>
#include <cassert>

namespace Util
{

[[nodiscard]] int GetRandomInt(int min, int max)
{
    static thread_local std::mt19937* gen = nullptr;
    if (!gen) {
        gen = new std::mt19937(static_cast<unsigned int>(clock() +
                               std::hash<std::thread::id>()(std::this_thread::get_id())));
    }
    std::uniform_int_distribution<int> dist(min, max);
    return dist(*gen);
}

[[nodiscard]] bool IsSquareWithinBoard(const Square& square)
{
    for (int num = 0; num < Square::NUM; num++) {

        Pos pos = square.GetPos(num);

        if (pos.GetRow() < 0 || pos.GetRow() >= Board::NUM_ROW ||
            pos.GetCol() < 0 || pos.GetCol() >= Board::NUM_COL) {
            return false;
        }
    }
    return true;
}

[[nodiscard]] bool IsBoardSquareSane(const Board& board, const Square& square)
{
    if (!IsSquareWithinBoard(square))
        return false;

    for (int num = 0; num < Square::NUM; num++) {

        Pos pos = square.GetPos(num);

        if (pos.GetRow() < 0 || pos.GetRow() >= Board::NUM_ROW ||
            pos.GetCol() < 0 || pos.GetCol() >= Board::NUM_COL) {
            return false;
        }

        if (board.GetTile(pos) == Board::Tile::WALL)
            return false;
    }
    return true;
}

[[nodiscard]] bool IsBoardSquareSolutionSane(const Board& board, const Square& square,
                                             const Solver::Solution& solution)
{
    if (!IsBoardSquareSane(board, square))
        return false;

    Solver::Solution sol = Solver::Solve(board, square, solution.GetDepth());
    if (!(sol == solution))
        return false;

    return true;
}

void CharVectorToBoardAndSquare(const std::vector<char>& charVec,
                                Board& board, Square& square)
{
    if (charVec.size() != Board::NUM_TILES)                     // Precondition check
        throw std::invalid_argument("Invalid charVec size");

    std::array<bool, Square::NUM> squareFound{ false, false, false, false };
    Board retBoard;
    Square retSquare;

    for (int num = 0; num < Board::NUM_TILES; num++) {

        char c = charVec.at(num);

        // Board is all Empty by default
        if (c == 'O')
            continue;

        Pos pos(num / Board::NUM_ROW, num % Board::NUM_ROW);

        if (c == 'X') {
            retBoard.SetTile(pos, Board::Tile::WALL);
        
        } else if (c >= 'A' && c <= 'D') {
            
            int idx = c - 'A';

            if (squareFound.at(idx))
                throw std::invalid_argument("charVec contains repeated squares");
            squareFound.at(idx) = true;

            retSquare.SetPos(idx, pos);

        } else {
            throw std::invalid_argument("charVec contains invalid character");
        }
    }

    board = std::move(retBoard);
    square = std::move(retSquare);
}

void BoardAndSquareToCharVector(const Board& board, const Square& square,
                                std::vector<char>& charVec)
{
    std::vector<char> retVec;
    retVec.reserve(Board::NUM_TILES);

    for (int row = 0; row < Board::NUM_ROW; row++) {
        for (int col = 0; col < Board::NUM_COL; col++) {

            Pos pos(row, col);

            if (board.GetTile(pos) == Board::Tile::EMPTY)
                retVec.emplace_back('O');
            else if (board.GetTile(pos) == Board::Tile::WALL)
                retVec.emplace_back('X');
        }
    }

    for (int num = 0; num < Square::NUM; num++) {
        Pos pos = square.GetPos(num);

        if (pos.GetRow() < 0 || pos.GetRow() >= Board::NUM_ROW ||
            pos.GetCol() < 0 || pos.GetCol() >= Board::NUM_COL)
            throw std::invalid_argument("Square pos not within board bounds");

        if (board.GetTile(pos) == Board::Tile::WALL)
            throw std::invalid_argument("Square pos coincides with board Wall");

        retVec.at(static_cast<size_t>(pos.GetRow()) * Board::NUM_ROW + pos.GetCol()) =
                'A' + static_cast<char>(num);
    }

    charVec = std::move(retVec);
}

} // namespace Util
