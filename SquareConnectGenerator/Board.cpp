/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#include "Board.hpp"

Board::Board(Tile tile) :
    _tiles{ NUM_TILES, tile }
{
    // Invariant check
    assert(_tiles.size() == NUM_TILES);
}

Board::Board(std::vector<Tile> tiles) :
    _tiles{ std::move(tiles) }
{
    // Invariant check
    assert(_tiles.size() == NUM_TILES);
}

Board::Tile Board::GetTile(const Pos& pos) const
{
    // Precondition check
    assert(pos.GetRow() >= 0 && pos.GetRow() < NUM_ROW);
    assert(pos.GetCol() >= 0 && pos.GetCol() < NUM_COL);
    // Invariant check
    assert(_tiles.size() == NUM_TILES);

    return _tiles.at((static_cast<size_t>(pos.GetRow()) * NUM_ROW) +
                     pos.GetCol());
}

Board& Board::SetTile(const Pos& pos, Tile tile)
{
    // Precondition check
    assert(pos.GetRow() >= 0 && pos.GetRow() < NUM_ROW);
    assert(pos.GetCol() >= 0 && pos.GetCol() < NUM_COL);
    // Invariant check
    assert(_tiles.size() == NUM_TILES);

    _tiles.at((static_cast<size_t>(pos.GetRow()) * NUM_ROW) +
              pos.GetCol()) = tile;

    // Invariant check
    assert(_tiles.size() == NUM_TILES);
    return *this;
}
