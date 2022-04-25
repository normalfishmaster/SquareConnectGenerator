/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#include "Pos.hpp"

#include <cmath>

Pos::Pos(int row, int col) :
    _row{ row },
    _col{ col }
{
}

Pos Pos::operator+(const Pos& pos) const
{
    return Pos(_row + pos._row, _col + pos._col);
}

Pos Pos::operator-(const Pos& pos) const
{
    return Pos(_row - pos._row, _col - pos._col);
}

Pos& Pos::operator+=(const Pos& pos)
{
    _row += pos._row;
    _col += pos._col;
    return *this;
}

Pos& Pos::operator-=(const Pos& pos)
{
    _row -= pos._row;
    _col -= pos._col;
    return *this;
}

bool Pos::operator==(const Pos& pos) const
{
    if (_row == pos._row && _col == pos._col)
        return true;
    return false;
}

bool Pos::operator!=(const Pos& pos) const
{
    if (_row == pos._row && _col == pos._col)
        return false;
    return true;
}

int Pos::GetRow() const
{
    return _row;
}

int Pos::GetCol() const
{
    return _col;
}

Pos& Pos::SetRow(int row)
{
    _row = row;
    return *this;
}

Pos& Pos::SetCol(int col)
{
    _col = col;
    return *this;
}

void Pos::IncRow(int val)
{
    _row += val;
}

void Pos::IncCol(int val)
{
    _col += val;
}

void Pos::IncRowAndCol(int row, int col)
{
    _row += row;
    _col += col;
}

bool Pos::IsAdjacent(const Pos& pos1, const Pos& pos2)
{
    int rowDiff = std::abs(pos1._row - pos2._row);
    int colDiff = std::abs(pos1._col - pos2._col);

    if (rowDiff == 0 && colDiff == 1)
        return true;
    else if (rowDiff == 1 && colDiff == 0)
        return true;

    return false;
}
