/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#include "Square.hpp"

#include <array>
#include <vector>
#include <utility>
#include <cassert>

#define INVARIANT_CHECK() \
    

Square::Square()
{
    _squares.reserve(NUM);
    _squares.emplace_back(Pos(-1, -1));
    _squares.emplace_back(Pos(-1, -2));
    _squares.emplace_back(Pos(-1, -3));
    _squares.emplace_back(Pos(-1, -4));

    // Invariant check
    assert(_squares.size() == NUM);
    assert(!IsPosRepeated(_squares));
}

Square::Square(std::vector<Pos> pos) :
    _squares{ std::move(pos) }
{
    // Invariant check
    assert(_squares.size() == NUM);
    assert(!IsPosRepeated(_squares));
}

Square::Equality Square::operator==(const Square& square) const
{
    // Precondition check
    assert(square._squares.size() == NUM);
    // Invariant check
    assert(_squares.size() == NUM);
    assert(!IsPosRepeated(_squares));

    std::array<bool, NUM> match{ false, false, false, false };
    int nExact = 0;
    int nSomewhat = 0;

    for (int n1 = 0; n1 < NUM; n1++) {
        for (int n2 = 0; n2 < NUM; n2++) {

            if (match.at(n2))
                continue;

            if (_squares.at(n1) == square._squares.at(n2)) {
                if (n1 == n2)
                    nExact++;
                else
                    nSomewhat++;

                match.at(n2) = true;
            }
        }
    }

    if (nExact == NUM && nSomewhat == 0)
        return Equality::PERFECTLY_EQUAL;
    else if(nExact + nSomewhat == NUM)
        return Equality::SOMEWHAT_EQUAL;
 
    return Equality::NOT_EQUAL;
}

Pos Square::GetPos(int num) const
{
    // Precondition check
    assert(num >= 0 && num < NUM);
    // Invariant check
    assert(_squares.size() == NUM);
    assert(!IsPosRepeated(_squares));

    return _squares.at(num);
}

bool Square::SetPos(int num, const Pos& pos)
{
    // Precondition check
    assert(num >= 0 && num < NUM);
    // Invariant check
    assert(_squares.size() == NUM);
    assert(!IsPosRepeated(_squares));

    for (int i = 0; i < NUM; i++) {
        if (i == num)
            continue;
        if (_squares.at(i) == pos) {
            INVARIANT_CHECK();
            return false;
        }
    }
    _squares.at(num) = pos;

    INVARIANT_CHECK();
    return true;
}

bool Square::SetPos(std::vector<Pos>& pos)
{
    // Precondition check
    assert(pos.size() == NUM);
    // Invariant check
    assert(_squares.size() == NUM);
    assert(!IsPosRepeated(_squares));

    if (IsPosRepeated(pos)) {
        INVARIANT_CHECK();
        return false;
    }
    _squares = pos;

    INVARIANT_CHECK();
    return true;
}

bool Square::SetPos(std::vector<Pos>&& pos)
{
    // Precondition check
    assert(pos.size() == NUM);
    // Invariant check
    assert(_squares.size() == NUM);
    assert(!IsPosRepeated(_squares));

    if (IsPosRepeated(pos))
        return false;
 
    _squares = std::move(pos);

    // Invariant check
    assert(_squares.size() == NUM);
    assert(!IsPosRepeated(_squares));
    return true;
}

int Square::IsPosSquare(const Pos& pos) const
{
    // Invariant check
    assert(_squares.size() == NUM);
    assert(!IsPosRepeated(_squares));

    for (int n = 0; n < NUM; n++) {
        if (_squares.at(n) == pos)
            return n;
    }
    return -1;
}

bool Square::IsSolved() const
{
    // Invariant check
    assert(_squares.size() == NUM);
    assert(!IsPosRepeated(_squares));

    int minRow, maxRow;
    int minCol, maxCol;

    minRow = maxRow = _squares.at(0).GetRow();
    minCol = maxCol = _squares.at(0).GetCol();

    for (int i = 1; i < NUM; i++) {

        int row = _squares.at(i).GetRow();
        int col = _squares.at(i).GetCol();

        // We can use if, else (rather than if, if) since
        // min and max as is initialized as the same value
        // before the for loop. This will not be possible if
        // they have been assigned different values.
        if (row < minRow)
            minRow = row;
        else if (row > maxRow)
            maxRow = row;

        if (col < minCol)
            minCol = col;
        else if (col > maxCol)
            maxCol = col;
    }

    if (maxRow - minRow == 1 && maxCol - minCol == 1)
        return true;

    return false;
}

bool Square::IsPosRepeated(const std::vector<Pos>& pos) const
{
    for (int n1 = 0; n1 < NUM; n1++) {
        for (int n2 = 0; n2 < NUM; n2++) {
            if (n1 == n2)
                continue;
            if (pos.at(n1) == pos.at(n2))
                return true;
        }
    }
    return false;
}
