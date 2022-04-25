/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "Filter.hpp"
#include "Solver.hpp"
#include "Board.hpp"
#include "Square.hpp"

namespace Generator
{

class Product
{
public:

    enum class Status : int
    {
        NONE = 0,
        SUCCESS,
        FAIL,
    };

    Product() = default;
    Product(Status status, int filterNum = 0,
            Board board = { }, Square square = { },
            Solver::Solution solution = { });
    Product(const Product& product) = default;
    Product(Product&& product) noexcept = default;

    ~Product() = default;

    Product& operator=(const Product& product) = default;
    Product& operator=(Product&& product) noexcept = default;

    Status GetStatus() const;
    int GetFilterNum() const;
    const Board& GetBoard() const;
    const Square& GetSquare() const;
    const Solver::Solution& GetSolution() const;

private:

    Status _status{ Status::NONE };
    int _filterNum{ 0 };
    Board _board{ };
    Square _square{ };
    Solver::Solution _solution{ };
};

[[nodiscard]] Product Generate(const Filter& filter);

} // namespace Generator

#endif // GENERATOR_HPP
