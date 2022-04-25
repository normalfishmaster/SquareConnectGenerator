/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#include "Movement.hpp"

#include "Util.hpp"
#include "Board.hpp"
#include "Square.hpp"
#include "Pos.hpp"

#include <cassert>

namespace Movement
{

Result::Result(bool success, Square square) :
    _success(success),
    _square(std::move(square))
{
}

bool Result::IsSuccess() const
{
    return _success;
}

const Square& Result::GetSquare() const
{
    return _square;
}

[[nodiscard]] Result Move(const Board& board, const Square& square, Dir dir)
{
    // Precondition check
    assert(dir != Dir::NONE);
    assert(Util::IsSquareWithinBoard(square));

    // Calculate new position

    std::vector<Pos> vPos;
    vPos.reserve(Square::NUM);

    for (int num = 0; num < Square::NUM; num++) {

        Pos pos = square.GetPos(num);
        int rowDelta = 0;
        int colDelta = 0;
        int iter = 0;

        if (dir == Dir::UP) {
            rowDelta = -1;
            iter = pos.GetRow();
        }
        else if (dir == Dir::DOWN) {
            rowDelta = 1;
            iter = Board::NUM_ROW - pos.GetRow() - 1;
        }
        else if (dir == Dir::LEFT) {
            colDelta = -1;
            iter = pos.GetCol();
        }
        else if (dir == Dir::RIGHT) {
            colDelta = 1;
            iter = Board::NUM_ROW - pos.GetCol() - 1;
        }

        int numSquareToWall = 0;

        for (int it = 0; it < iter; it++) {

            pos.IncRow(rowDelta);
            pos.IncCol(colDelta);

            if (board.GetTile(pos) == Board::Tile::WALL) {
                pos.IncRow(-1 * rowDelta);
                pos.IncCol(-1 * colDelta);
                break;
            }

            if (square.IsPosSquare(pos) >= 0)
                numSquareToWall++;
        }

        pos.IncRow(-1 * rowDelta * numSquareToWall);
        pos.IncCol(-1 * colDelta * numSquareToWall);
        vPos.emplace_back(pos);
    }

    Square squareNew(std::move(vPos));
    bool success = ((squareNew == square) ==
                   Square::Equality::PERFECTLY_EQUAL ? false : true);

    return Result(success, std::move(squareNew));
}

} // namespace Movement
