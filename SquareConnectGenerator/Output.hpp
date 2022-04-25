/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include "Solver.hpp"
#include "Board.hpp"
#include "Square.hpp"

#include <string>

class Output
{
public:

    Output();
    Output(const Output& output) = delete;
    Output(Output&& output) noexcept = delete;

    ~Output() = default;

    Output& operator=(const Output& output) = delete;
    Output& operator=(Output&& output) noexcept = delete;

    void AppendToFile(const std::string& subDir, const std::string& fileName, int count,
                      const Board& board, const Square& square,
                      const Solver::Solution& solution) const;

private:

    std::string _outputDir{ };
};

#endif // OUTPUT_HPP
