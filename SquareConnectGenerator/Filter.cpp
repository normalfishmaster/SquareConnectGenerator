/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#include "Filter.hpp"

#include "Profiler.hpp"
#include "Solver.hpp"
#include "Util.hpp"
#include "Board.hpp"
#include "Square.hpp"

#include <string>
#include <vector>
#include <memory>
#include <cassert>

void Filter::AddEntry(const std::string& title,
                      int depth,
                      const Profiler::MatchProfile* profile)
{
    // Precondition check
    assert(title.length() > 0);
    assert(depth > 0);

    Entry entry;
    entry._title = title;
    entry._depth = depth;
    entry._profile = std::make_unique<Profiler::MatchProfile>(*profile);

    _entry.push_back(std::move(entry));

    if (depth > _maxDepth)
        _maxDepth = depth;
}

[[nodiscard]] int Filter::MatchFilter(const Board& board, const Square& square,
                                      const Solver::Solution& solution) const
{
    // Precondition check
    assert(solution.GetStatus() == Solver::Solution::Status::SOLVED);
    assert(Util::IsBoardSquareSolutionSane(board, square, solution));

    // These arguments are not used for the moment but will likely
    // be used if the Filter features are expanded in the future.
    (void)board;
    (void)square;

    int depth = solution.GetDepth();
    Profiler::AnalysisProfile ana = Profiler::Analyze(solution.GetSquare());

    for (int entNum = 0; entNum < _entry.size(); entNum++) {

        const Entry& ent = _entry.at(entNum);

        if (ent._depth == depth &&
            (ent._profile != nullptr && Profiler::Match(ana, *(ent._profile))))
            return entNum;
    }

    return -1;
}

int Filter::GetNumEntry() const
{
    return static_cast<int>(_entry.size());
}

const std::string& Filter::GetEntryTitle(int num) const
{
    return _entry.at(num)._title;
}

int Filter::GetMaxDepth() const
{
    return _maxDepth;
}
