/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "Pos.hpp"

#include <vector>
#include <utility>
#include <exception>

class Square
{
public:

    static const int NUM = 4;

    enum class Equality : int
    {
        NOT_EQUAL = 0,
        SOMEWHAT_EQUAL = 1,
        PERFECTLY_EQUAL = 2,
    };

    Square();
    Square(std::vector<Pos> pos);
    Square(const Square& square) = default;
    Square(Square&& square) noexcept = default;

    ~Square() = default;

    Square& operator=(const Square& square) = default;
    Square& operator=(Square&& square) noexcept = default;

    Equality operator==(const Square& square) const;

    Pos GetPos(int num) const;
    bool SetPos(int num, const Pos& pos);
    bool SetPos(std::vector<Pos>& pos);
    bool SetPos(std::vector<Pos>&& pos);

    int IsPosSquare(const Pos& pos) const;
    bool IsSolved() const;

private:

    // Use vector instead of array as the former is movable
    std::vector<Pos> _squares;

    bool IsPosRepeated(const std::vector<Pos>& s) const;
};

#endif // SQUARE_HPP
