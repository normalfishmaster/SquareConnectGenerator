/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#include "Generator.hpp"

#include "Util.hpp"
#include "Filter.hpp"
#include "Solver.hpp"
#include "Board.hpp"
#include "Square.hpp"
#include "Pos.hpp"

#include <vector>
#include <iostream>

namespace Generator
{

Product::Product(Status status, int filterNum,
                 Board board, Square square,
                 Solver::Solution solution) :
    _status(status), _filterNum(filterNum),
    _board(std::move(board)), _square(std::move(square)),
    _solution(std::move(solution))
{
}

Product::Status Product::GetStatus() const
{
    return _status;
}

int Product::GetFilterNum() const
{
    return _filterNum;
}

const Board& Product::GetBoard() const
{
    return _board;
}

const Square& Product::GetSquare() const
{
    return _square;
}

const Solver::Solution& Product::GetSolution() const
{
    return _solution;
}

[[nodiscard]] static Square GenerateSquare()
{
    while (1) {

        std::vector<Pos> posVec;
        posVec.reserve(Square::NUM);

        for (int num = 0; num < Square::NUM; num++) {

            bool found = false;

            while (1) {

                Pos pos(Util::GetRandomInt(0, Board::NUM_ROW - 1),
                    Util::GetRandomInt(0, Board::NUM_COL - 1));

                found = false;

                for (int pNum = 0; pNum < posVec.size(); pNum++) {
                    if (posVec.at(pNum) == pos) {
                        found = true;
                        break;
                    }
                }
                if (found)
                    continue;

                posVec.emplace_back(pos);
                break;
            }
        }

        Square sqr(std::move(posVec));
        if (!sqr.IsSolved())
            return sqr;
    }
}

[[nodiscard]] static std::vector<int> GenerateWallStack()
{
    std::vector<int> retWall;
    retWall.reserve(Board::NUM_TILES);

    for (int num = 0; num < Board::NUM_TILES; num++)
        retWall.emplace_back(num);

    int numSwap = Board::NUM_TILES / 2;

    for (int num = 0; num < numSwap; num++) {
        std::swap(retWall.at(Util::GetRandomInt(0, Board::NUM_TILES - 1)),
            retWall.at(Util::GetRandomInt(0, Board::NUM_TILES - 1)));
    }

    return retWall;
}

void FillUnreachableTiles(Board& board, const Square& square)
{
    // Precondition check
    assert(Util::IsBoardSquareSane(board, square));

    Board retBrd(Board::Tile::WALL);
    std::vector<Pos> node;
    node.reserve(Board::NUM_TILES);
    int nodeCnt = 0;

    node.emplace_back(square.GetPos(0));
    retBrd.SetTile(square.GetPos(0), Board::Tile::EMPTY);

    while (nodeCnt != node.size()) {

        const Pos& pos = node.at(nodeCnt);

        std::array<Pos, 4> nPos{ {
            pos - Pos(1, 0),    // Up
            pos + Pos(1, 0),    // Down
            pos - Pos(0, 1),    // Left
            pos + Pos(0, 1),    // Right
        } };

        for (const auto& p : nPos) {

            if (p.GetRow() < 0 || p.GetRow() >= Board::NUM_ROW ||
                p.GetCol() < 0 || p.GetCol() >= Board::NUM_COL)
                continue;

            if (retBrd.GetTile(p) == Board::Tile::EMPTY)
                continue;

            if (board.GetTile(p) == Board::Tile::EMPTY) {
                node.emplace_back(p);
                retBrd.SetTile(p, Board::Tile::EMPTY);
            }
        }

        nodeCnt++;
    }

    board = std::move(retBrd);
}

void FillInconsequentialTiles(Board& board, const Solver::Solution& solution)
{
    for (int r = 0; r < Board::NUM_ROW; r++) {
        for (int c = 0; c < Board::NUM_COL; c++) {
            
            Pos p(r, c);

            if (board.GetTile(p) == Board::Tile::WALL)
                continue;

            const std::vector<Square>& sqr = solution.GetSquare();
            if (sqr.at(0).IsPosSquare(p) >= 0)
                continue;

            board.SetTile(p, Board::Tile::WALL);

            Solver::Solution sol = Solver::Solve(board, sqr.at(0), solution.GetDepth());
            if (sol != solution)
                board.SetTile(p, Board::Tile::EMPTY);
        }
    }
}

[[nodiscard]] Product Generate(const Filter& filter)
{
    // Precondition check
    assert(filter.GetNumEntry() > 0);

    Square sqr = GenerateSquare();
    Board brd;

    std::vector<int> wall = GenerateWallStack();
    // Do not attempt so solve before 5% of the number of available tiles
    // as the puzzle very likely not solvable or meet any filter criterias.
    int minWall = static_cast<int>(0.05 * Board::NUM_TILES);
    // Do not generate any more than 80% of the number of available tiles
    // as the puzzle would very likely not be solvable or meet any filter
    // criterias.
    int maxWall = static_cast<int>(0.8 * Board::NUM_TILES);
    int wallCnt = 0;

    Solver::Solution lastSol;

    while (wallCnt < maxWall) {

        Pos pos(wall.at(wallCnt) / Board::NUM_ROW,
                wall.at(wallCnt) % Board::NUM_ROW);
        wallCnt++;

        if (sqr.IsPosSquare(pos) >= 0)
            continue;

        brd.SetTile(pos, Board::Tile::WALL);

        if (wallCnt < minWall)
            continue;

        Solver::Solution sol = Solver::Solve(brd, sqr, filter.GetMaxDepth());
        if (sol.GetStatus() != Solver::Solution::Status::SOLVED)
            continue;

        // Do not perform filter matching if the solution is the same as before
        // to avoid wasting processing time.
        if (sol == lastSol)
            continue;

        int filNum = filter.MatchFilter(brd, sqr, sol);
        if (filNum >= 0) {
            FillInconsequentialTiles(brd, sol);
            FillUnreachableTiles(brd, sqr);
            return Product(Product::Status::SUCCESS, filNum,
                           std::move(brd), std::move(sqr),
                           std::move(sol));
        }

        lastSol = std::move(sol);
    }

    return Product(Product::Status::FAIL);
}

} // namespace Generator
