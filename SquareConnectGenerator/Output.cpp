/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#include "Output.hpp"

#include "Solver.hpp"
#include "Util.hpp"
#include "Board.hpp"
#include "Square.hpp"

#include <filesystem>
#include <exception>
#include <fstream>
#include <string>
#include <format>
#include <ctime>

Output::Output()
{
    time_t t;
    struct tm tm;

    time(&t);
    if (localtime_s(&tm, &t))
        throw std::runtime_error("Calling localtime_s failed");

    _outputDir = std::format("output/{:4}.{:02}.{:02}__({:02}.{:02}.{:02})",
                             tm.tm_year + 1900,
                             tm.tm_mon + 1,
                             tm.tm_mday,
                             tm.tm_hour,
                             tm.tm_min,
                             tm.tm_sec);

    try {
        if (!std::filesystem::create_directories(_outputDir))
            throw std::runtime_error("Creating output directory failed");
    } catch (...) {
        throw;
    }
}

void Output::AppendToFile(const std::string& subDir, const std::string& fileName, int count,
                          const Board& board, const Square& square,
                          const Solver::Solution& solution) const
{
    // Precondition check
    assert(subDir.length() > 0);
    assert(fileName.length() > 0);
    assert(count >= 0);
    assert(solution.GetStatus() == Solver::Solution::Status::SOLVED);
    assert(Util::IsBoardSquareSolutionSane(board, square, solution));

    std::string subDirPath = _outputDir + "/" + subDir;
    std::string filePath = subDirPath + "/" + fileName;

    try {
        std::filesystem::create_directories(subDirPath);
    } catch (...) {
        throw;
    }

    const std::vector<Movement::Dir>& dir = solution.GetDir();

    std::ofstream ofs;
    ofs.open(filePath, std::ios_base::app);
    if (!ofs.is_open())
        throw std::runtime_error(std::string("Unable to open file:") + filePath);

    ofs << "\t\t\t// Puzzle " << count << std::endl;
    ofs << std::endl;
    ofs << "\t\t\tnew Map()" << std::endl;
    ofs << "\t\t\t{" << std::endl;
    ofs << "\t\t\t\t_layout = new sbyte[,]" << std::endl;
    ofs << "\t\t\t\t{" << std::endl;

    for (int row = 0; row < Board::NUM_ROW; row++)
    {
        ofs << "\t\t\t\t\t{ ";

        for (int col = 0; col < Board::NUM_COL; col++)
        {
            Pos pos(row, col);
            Board::Tile tile = board.GetTile(pos);
            int sqrIdx;

            if (tile == Board::Tile::WALL) {
                ofs << "X, ";
                continue;
            }
            
            if ((sqrIdx = square.IsPosSquare(pos)) < 0) {
                ofs << "0, ";
                continue;
            }

            if (sqrIdx == 0)
                ofs << "A, ";
            else if (sqrIdx == 1)
                ofs << "B, ";
            else if (sqrIdx == 2)
                ofs << "C, ";
            else if (sqrIdx == 3)
                ofs << "D, ";
        }

        ofs << "}," << std::endl;
    }

    ofs << "\t\t\t\t}," << std::endl;
    ofs << std::endl;
    ofs << "\t\t\t\t_hint = new char[] { ";

    for (int num = 0; num < dir.size(); num++)
    {
        if (dir.at(num) == Movement::Dir::UP) {
            ofs << "'U',";
            continue;
        }
        else if (dir.at(num) == Movement::Dir::DOWN) {
            ofs << "'D',";
            continue;
        }
        else if (dir.at(num) == Movement::Dir::LEFT) {
            ofs << "'L',";
            continue;
        }
        else if (dir.at(num) == Movement::Dir::RIGHT) {
            ofs << "'R',";
            continue;
        }
    }

    ofs << " }," << std::endl;
    ofs << "\t\t\t}," << std::endl;
    ofs << std::endl;

    ofs.close();
}
