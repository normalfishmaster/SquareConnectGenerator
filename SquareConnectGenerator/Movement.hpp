/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP

#include "Board.hpp"
#include "Square.hpp"

namespace Movement
{

enum class Dir : int
{
    NONE = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Result
{
public:

    Result() = default;
    Result(bool success, Square square);
    Result(const Result& result) = default;
    Result(Result&& result) noexcept = default;

    ~Result() = default;

    Result& operator=(const Result& result) = default;
    Result& operator=(Result&& result) noexcept = default;

    bool IsSuccess() const;
    const Square& GetSquare() const;

private:

    bool _success{ false };
    Square _square{ };
};

[[nodiscard]] Result Move(const Board& board, const Square& old, Dir dir);

}; // namespace Movement

#endif // MOVEMENT_HPP
