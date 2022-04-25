/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#include "Profiler.hpp"

#include "Util.hpp"
#include "Square.hpp"

#include <map>
#include <array>
#include <vector>
#include <algorithm>
#include <cassert>

#include <iostream>

namespace Profiler
{

Island::Island()
{
    _squareIdx.reserve(Square::NUM);

    // Invariant check
    assert(_squareIdx.size() <= Square::NUM);
}

bool Island::operator<(const Island& island) const
{
    // Invariant check
    assert(_squareIdx.size() <= Square::NUM);

    return _squareIdx < island._squareIdx;
}

bool Island::operator==(const Island& island) const
{
    // Invariant check
    assert(_squareIdx.size() <= Square::NUM);

    return _squareIdx == island._squareIdx;
}

bool Island::operator!=(const Island& island) const
{
    // Invariant check
    assert(_squareIdx.size() <= Square::NUM);

    return _squareIdx != island._squareIdx;
}

int Island::GetSize(void) const
{
    // Invariant check
    assert(_squareIdx.size() <= Square::NUM);

    return static_cast<int>(_squareIdx.size());
}

int Island::GetSquareIdx(int islandIdx) const
{
    // Precondition check
    assert(islandIdx < _squareIdx.size());
    // Invariant check
    assert(_squareIdx.size() <= Square::NUM);

    return _squareIdx.at(islandIdx);
}

void Island::AddSquareIdx(int squareIdx)
{
    // Precondition check
    assert(_squareIdx.size() <= Square::NUM - 1);
    // Invariant check
    assert(_squareIdx.size() <= Square::NUM);

    _squareIdx.emplace_back(squareIdx);

    // Invariant check
    assert(_squareIdx.size() <= Square::NUM);
}

int GetTypeSize(Type type)
{
    if (type == Type::SINGLET_O)
        return SINGLET_SIZE;
    else if (type == Type::DUPLET_I)
        return DUPLET_SIZE;
    else if (type >= Type::TRIPLET_L && type <= Type::TRIPLET_I)
        return TRIPLET_SIZE;
    else if (type >= Type::QUARTET_H && type <= Type::QUARTET_I)
        return TRIPLET_SIZE;

    assert(0);
    return -1;
}

AnalysisProfile::AnalysisProfile(int numEntry)
{
    _entry.reserve(numEntry);
}

int AnalysisProfile::GetNumEntry() const
{
    return static_cast<int>(_entry.size());
}

const std::vector<TypeIsland>& AnalysisProfile::GetEntryTypeIsland(int num) const
{
    // Precondition check
    assert(num >= 0 && num < _entry.size());

    return _entry.at(num)._typeIsland;
}

void AnalysisProfile::AddEntry(std::vector<TypeIsland> typeIs)
{
    // Precondition check
    assert(_entry.size() + 1 <= _entry.capacity());

    Entry newEntry;
    newEntry._typeIsland = std::move(typeIs);

    _entry.emplace_back(newEntry);
}

int MatchProfile::GetNumEntry() const
{
    return static_cast<int>(_entry.size());
}

Mode MatchProfile::GetEntryMode(int num) const
{
    // Precondition check
    assert(num >= 0 && num < _entry.size());
    return _entry.at(num)._mode;
}

const std::vector<TypeIdentity>& MatchProfile::GetEntryTypeIdentity(int num) const
{
    // Precondition check
    assert(num >= 0 && num < _entry.size());
    return _entry.at(num)._typeIdentity;
}

void MatchProfile::AddEntry(Mode mode, std::vector<TypeIdentity> typeId)
{
    // Precondition check
    assert((mode == Mode::COMPARE && typeId.size() > 0) ||
        (mode != Mode::COMPARE && typeId.size() == 0));
    assert(ValidateTypeIdentity(typeId));

    // Sort by the largest Type size first, then by smallest Identity value first.
    // This helps simplify matching profiles such that out of order TypeIdentities
    // do not need to be handled.
    std::sort(
        typeId.begin(), typeId.end(),
        [](TypeIdentity& t1, TypeIdentity& t2) {
            if (t1.first != t2.first)
                return t1.first < t2.first;
            return t1.second < t2.second;
        }
    );

    Entry newEntry;
    newEntry._mode = mode;
    newEntry._typeIdentity = std::move(typeId);

    _entry.push_back(std::move(newEntry));
}

// Check for the following:
// 1. Total size does not exceep Square::NUM.
// 2. No identities are repeated.
// 3. The TypeIdentity pair are consistent with existing entries.
bool MatchProfile::ValidateTypeIdentity(const std::vector<TypeIdentity>& typeId) const
{
    int size = 0;

    for (int tIdNum0 = 0; tIdNum0 < typeId.size(); tIdNum0++) {

        size += GetTypeSize(typeId.at(tIdNum0).first);

        if (typeId.at(tIdNum0).second == Identity::ANY)
            continue;

        for (int tIdNum1 = 0; tIdNum1 < typeId.size(); tIdNum1++) {
            if (tIdNum0 == tIdNum1)
                continue;
            if (typeId.at(tIdNum0).second == typeId.at(tIdNum1).second)
                return false;
        }

        for (const Entry& ent : _entry) {
            for (const auto& tId : ent._typeIdentity) {
                if (typeId.at(tIdNum0).second == tId.second &&
                    typeId.at(tIdNum0).first != tId.first)
                    return false;
            }
        }
    }
    if (size > Square::NUM)
        return false;
    return true;
}

// Classify Square into Islands using Breadth First Search.  A vector of Islands
// is returned back to the caller.  The vector is sorted by largest size first,
// followed by the smallest content first (Square A first rather than Square B). 
[[nodiscard]] std::vector<Island> IdentifyIslands(const Square& square)
{
    // Precondition check
    assert(Util::IsSquareWithinBoard(square));

    // Track squares which belong to the same island with the same number starting
    // from 0.  Squares whidh does not yet belong to an island is marked with -1.
    std::array<int, Square::NUM> islandTracker{ -1, -1, -1, -1 };
    int islandCount = 0;

    for (int num0 = 0; num0 < Square::NUM; num0++) {

        if (islandTracker.at(num0) != -1)
            continue;

        // We could only get at a maximum of Square::NUM nodes
        std::array<int, Square::NUM> node{ 0, 0, 0, 0 };
        int nodeCount = 0;
        int nodeSize = 0;

        node.at(nodeSize++) = num0;
        islandTracker.at(num0) = islandCount;

        while (nodeCount != nodeSize) {

            int sqrNum = node.at(nodeCount);
            Pos sqrPos = square.GetPos(sqrNum);

            for (int num1 = 0; num1 < Square::NUM; num1++) {

                if (num0 == num1)
                    continue;

                if (islandTracker.at(num1) != -1)
                    continue;

                if (Pos::IsAdjacent(sqrPos, square.GetPos(num1))) {
                    node.at(nodeSize++) = num1;
                    islandTracker.at(num1) = islandCount;
                }
            }
            nodeCount++;
        }
        islandCount++;
    }

    assert(islandTracker.at(0) >= 0);
    assert(islandTracker.at(1) >= 0);
    assert(islandTracker.at(2) >= 0);
    assert(islandTracker.at(3) >= 0);

    // Construct return vector

    std::vector<Island> retVec;
    retVec.reserve(islandCount);

    for (int cnt0 = 0; cnt0 < islandCount; cnt0++) {

        Island island;

        for (int cnt1 = 0; cnt1 < (int)islandTracker.size(); cnt1++) {
            if (cnt0 == islandTracker.at(cnt1))
                island.AddSquareIdx(cnt1);
        }
        retVec.emplace_back(island);
    }

    std::sort(
        retVec.begin(), retVec.end(),
        [](Island& i1, Island& i2) {
            if (i1.GetSize() != i2.GetSize())
                return (i1.GetSize() > i2.GetSize());
            return (i1.GetSquareIdx(0) < i2.GetSquareIdx(0));
        }
    );

    return retVec;
}

// Truth table for detecting types
// Detection is processed from left to right.
// (X = irrelevant)
// 
// Type        :  1O    2I    3I    3L    4H    4I    4S    4T    4L
// 
// Size        :  1     2     3     3     4     4     4     4     4
// Row Delta   :  X     X     2/0   1/1   2/2   3/0   2/0   2/0   1/2
// Col Delta   :  X     X     0/2   1/1   2/2   0/3   0/2   2/0   2/1
// Num Min Row :  X     X     X     X     X     X     2/1   1/1   1/2
// Num Max Row :  X     X     X     X     X     X     2/1   3/1   2/1
// Num Min Col :  X     X     X     X     X     X     1/2   1/1   1/3
// Num Max Col :  X     X     X     X     X     X     1/2   1/3   3/1
//
[[nodiscard]] AnalysisProfile Analyze(const std::vector<Square>& square)
{
    AnalysisProfile profile(static_cast<int>(square.size()));

    // Create type list

    for (const auto& sq : square) {

        std::vector<Island> island(IdentifyIslands(sq));
        std::vector<TypeIsland> typeIsland;
        typeIsland.reserve(Square::NUM);

        for (auto& is : island) {

            int isSize = is.GetSize();

            if (isSize == SINGLET_SIZE) {
                typeIsland.emplace_back(TypeIsland(Type::SINGLET_O, std::move(is)));
                continue;
            }

            if (isSize == DUPLET_SIZE) {
                typeIsland.emplace_back(TypeIsland(Type::DUPLET_I, std::move(is)));
                continue;
            }

            // Only triplets or quartets beyond this point

            int rowDelta, colDelta;
            int minRow, maxRow, minCol, maxCol;
            int numMinRow, numMaxRow, numMinCol, numMaxCol;

            Pos pos = sq.GetPos(is.GetSquareIdx(0));
            int row = pos.GetRow();
            int col = pos.GetCol();

            minRow = maxRow = row;
            minCol = maxCol = col;
            numMinRow = numMaxRow = numMinCol = numMaxCol = 1;

            for (int sz = 1; sz < isSize; sz++) {

                pos = sq.GetPos(is.GetSquareIdx(sz));
                row = pos.GetRow();
                col = pos.GetCol();

                // We can use if, else (rather than if, if) since
                // min and max is initialized as the same value
                // before the for loop. This will not be possible if
                // they have been assigned different values.
                if (row < minRow) {
                    minRow = row;
                    numMinRow = 1;
                } else if (row > maxRow) {
                    maxRow = row;
                    numMaxRow = 1;
                } else {
                    if (row == minRow)
                        numMinRow++;
                    if (row == maxRow)
                        numMaxRow++;
                }

                if (col < minCol) {
                    minCol = col;
                    numMinCol = 1;
                } else if (col > maxCol) {
                    maxCol = col;
                    numMaxCol = 1;
                } else {
                    if (col == minCol)
                        numMinCol++;
                    if (col == maxCol)
                        numMaxCol++;
                }
            }

            rowDelta = std::abs(maxRow - minRow);
            colDelta = std::abs(maxCol - minCol);

            // Process triplets

            if (isSize == TRIPLET_SIZE) {
                if ((rowDelta == 2 && colDelta == 0) ||
                    (rowDelta == 0 && colDelta == 2)) {
                    typeIsland.emplace_back(TypeIsland(Type::TRIPLET_I, std::move(is)));
                    continue;
                } else {
                    typeIsland.emplace_back(TypeIsland(Type::TRIPLET_L, std::move(is)));
                    continue;
                }
            }

            // Only quartets beyond this point

            if (rowDelta == 1 && colDelta == 1) {
                typeIsland.emplace_back(TypeIsland(Type::QUARTET_H, std::move(is)));
                continue;
            }

            if ((rowDelta == 3 && colDelta == 0) ||
                (rowDelta == 0 && colDelta == 3)) {
                typeIsland.emplace_back(TypeIsland(Type::QUARTET_I, std::move(is)));
                continue;
            }

            if ((numMinCol == 2 && numMaxCol == 2) ||
                (numMinRow == 2 && numMaxRow == 2)) {
                typeIsland.emplace_back(TypeIsland(Type::QUARTET_S, std::move(is)));
                continue;
            }

            if ((numMinCol == 1 && numMaxCol == 1) ||
                (numMinRow == 1 && numMaxRow == 1)) {
                typeIsland.emplace_back(TypeIsland(Type::QUARTET_T, std::move(is)));
                continue;
            }

            typeIsland.emplace_back(TypeIsland(Type::QUARTET_L, std::move(is)));
        }
        profile.AddEntry(std::move(typeIsland));
    }

    // Postcondition check
    assert(profile.GetNumEntry() == square.size());
    return profile;
}

// There is no need to handle out of order TypeIdentities and TypeIslands as they
// have been sorted before hand.
[[nodiscard]] static bool MatchTypeIdentityToTypeIsland(
    const std::vector<TypeIdentity>& typeIdentity,
    const std::vector<TypeIsland>& typeIsland,
    std::map<Identity, Island>& identityIsland)
{
    // Precondition check
    assert(typeIdentity.size() > 0);
    assert(typeIsland.size() > 0);

    if (typeIsland.size() < typeIdentity.size())
        return false;

    std::map<Identity, Island>& identityIslandCopy = identityIsland;

    // Use array as at most, there can only be four Identities and Islands.
    // Tracks if a TypeIdentity has found a match.
    std::array<bool, Square::NUM> found{ false, false, false, false };
    // Tracks if a TypeIsland has been matched to a TypeIdentity.
    std::array<bool, Square::NUM> used{ false, false, false, false };

    // Perform two passes.
    // Pass 0) Ignore all Identities that cannot be found in IdentityIsland.
    //         This allows Identities which have a match to be matched first.
    //         This avoids the use of a TypeIsland which would otherwise
    //         be used for an actual match.
    // Pass 1) Process all Identities that cannot be found in Pass 1.
    for (int pass = 0; pass < 2; pass++) {

        for (int tIdNum = 0; tIdNum < typeIdentity.size(); tIdNum++) {

            if (found.at(tIdNum))
                continue;

            const TypeIdentity& tId = typeIdentity.at(tIdNum);

            for (int tIsNum = 0; tIsNum < typeIsland.size(); tIsNum++) {

                if (used.at(tIsNum))
                    continue;

                const TypeIsland& tIs = typeIsland.at(tIsNum);

                if (tId.first != tIs.first)
                    continue;

                // We should always handle the concrete identities before
                // ANY.  However, no special handling is required here as
                // ANY is always sorted to the back in a MatchProfile.
                if (tId.second != Identity::ANY) {
                    auto iter = identityIslandCopy.find(tId.second);
                    if (iter == identityIslandCopy.end()) {
                        if (pass == 0)
                            break;

                        // Check if Island has been mapped to another
                        // Identity.
                        for (const auto& idIs : identityIslandCopy) {
                            if (idIs.second == tIs.second)
                                return false;
                        }

                        identityIslandCopy.insert({ tId.second, tIs.second });
                    } else {
                        if (iter->second != tIs.second)
                            continue;
                    }
                }

                found.at(tIdNum) = true;
                used.at(tIsNum) = true;
                break;
            }

            if (!found.at(tIdNum) && pass == 1)
                return false;
        }
    }

    identityIsland = std::move(identityIslandCopy);
    return true;
}

// The following are the rules for multiple skips stacked together:
// 1. A (n * SKIP_1) followed by SKIP_0_or_N guarantees at least n skips.
// 2. A SKIP_0_or_N followed by (n * SKIP_1) will perform exactly n skips
//    (The SKIP_0_N does not take effect).
[[nodiscard]] bool Match(const AnalysisProfile& analysis, const MatchProfile& match)
{
    // Precondition check
    assert(analysis.GetNumEntry() > 0);
    assert(match.GetNumEntry() > 0);

    std::map<Identity, Island> idIs;
    Mode lastMode = Mode::COMPARE;
    bool skip0N = false;
    int anaNum = 0;

    int numMatchEntry = match.GetNumEntry();
    int numAnalysisEntry = analysis.GetNumEntry();

    for (int matNum = 0; matNum < numMatchEntry; matNum++) {

        Mode mode = match.GetEntryMode(matNum);
        lastMode = mode;

        if (mode == Mode::SKIP_0_OR_N) {
            skip0N = true;
            continue;
        } else if (mode == Mode::SKIP_1) {
            skip0N = false;
            anaNum++;
            if (anaNum > numAnalysisEntry)
                return false;
            continue;
        }

        // Only Mode::COMPARE beyond this point

        if (anaNum >= numAnalysisEntry)
            return false;

        const std::vector<TypeIdentity>& tId = match.GetEntryTypeIdentity(matNum);

        for (; anaNum < numAnalysisEntry; anaNum++) {

            const std::vector<TypeIsland>& tIs = analysis.GetEntryTypeIsland(anaNum);

            if (MatchTypeIdentityToTypeIsland(tId, tIs, idIs)) {
                skip0N = false;
                anaNum++;
                break;
            }

            if (skip0N)
                continue;

            return false;
        }
    }

    if (anaNum < numAnalysisEntry) {
        if (lastMode == Mode::SKIP_0_OR_N)
            return true;
        else
            return false;
    }

    return true;
}

} // namespace Profiler