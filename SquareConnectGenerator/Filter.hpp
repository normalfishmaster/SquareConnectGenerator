/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#ifndef FILTER_HPP
#define FILTER_HPP

#include "Profiler.hpp"
#include "Solver.hpp"
#include "Board.hpp"
#include "Square.hpp"

#include <string>
#include <vector>
#include <memory>

class Filter
{
public:

    Filter() = default;
    Filter(const Filter& filter) = delete;
    Filter(Filter&& filter) noexcept = delete;

    ~Filter() = default;

    Filter& operator=(const Filter& filter) = delete;
    Filter& operator=(Filter&& filter) noexcept = delete;

    void AddEntry(const std::string& title,
                 int depth,
                 const Profiler::MatchProfile* profile = nullptr);

    [[nodiscard]] int MatchFilter(const Board& board, const Square& square,
                                  const Solver::Solution& solution) const;

    int GetNumEntry() const;

    const std::string& GetEntryTitle(int num) const;

    int GetMaxDepth() const;

private:

    struct Entry
    {
        std::string _title{ };
        int _depth{ 0 };
        std::unique_ptr<Profiler::MatchProfile> _profile{ nullptr };
    };

    std::vector<Entry> _entry{ };
    int _maxDepth{ 0 };
};

#endif // FILTER_HPP
