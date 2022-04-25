/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#ifndef BOARD_HPP
#define BOARD_HPP

#include "Pos.hpp"

#include <vector>
#include <cassert>

class Board
{
public:

    static const int NUM_ROW = 8;
    static const int NUM_COL = 8;
    static const int NUM_TILES = NUM_ROW * NUM_COL;

    enum class Tile : int
    {
        EMPTY = 0,
        WALL,
    };

    Board() = default;
    Board(Tile tile);
    Board(std::vector<Tile> tiles);
    Board(const Board& board) = default;
    Board(Board&& board) noexcept = default;

    ~Board() = default;

    Board& operator=(const Board& board) = default;
    Board& operator=(Board&& board) noexcept = default;

    Tile GetTile(const Pos& pos) const;
    Board& SetTile(const Pos& pos, Tile tile);

private:

    // Use vector array instead of array as the former is movable.
    // Use 1d array as initialization is faster.
    std::vector<Tile> _tiles{ NUM_TILES, Tile::EMPTY };
};

#endif // BOARD_HPP
