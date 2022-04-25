/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#ifndef PROFILER_HPP
#define PROFILER_HPP

#include "Square.hpp"

#include <array>
#include <vector>
#include <utility>

namespace Profiler
{

static const int QUARTET_SIZE = 4;
static const int TRIPLET_SIZE = 3;
static const int DUPLET_SIZE = 2;
static const int SINGLET_SIZE = 1;

enum class Mode : int
{
    COMPARE = 0,
    SKIP_0_OR_N,
    SKIP_1,
};

// Visual guide:
// 
// Quartet
//      H) XX       S)  XX      T) XXX      L) XXX      I) XXXX
//         XX           XX          X            X
// 
// Triplet
//      L) XX       I) XXX
//         X
//
// Duplet
//      I) XX
//
// Singlet
//      O) X
// 
enum class Type : int
{
    QUARTET_H = 0,
    QUARTET_S,
    QUARTET_T,
    QUARTET_L,
    QUARTET_I,
    TRIPLET_L,
    TRIPLET_I,
    DUPLET_I,
    SINGLET_O,
};

// An identity identifies a unique island.
// An island is a unique group of squares which are adjacent to 
// one another.
enum class Identity : int
{
    A = 0,
    B, C, D, E, F,
    G, H, I, J, K,
    L, M, N, O, P,
    Q, R, S, T, U,
    V, W, X, Y, Z,
    ANY,
};

// An island is a unique group of squares which are adjacent to 
// one another.
class Island
{
public:

    Island();
    Island(const Island& island) = default;
    Island(Island&& island) noexcept = default;

    ~Island() = default;

    Island& operator=(const Island& island) = default;
    Island& operator=(Island&& island) noexcept = default;

    bool operator<(const Island& island) const;
    bool operator==(const Island& island) const;
    bool operator!=(const Island& island) const;

    int GetSize(void) const;
    int GetSquareIdx(int islandIdx) const;
    void AddSquareIdx(int squareIdx);

private:

    std::vector<int> _squareIdx{ };
};

using TypeIdentity = std::pair<Type, Identity>;
using TypeIsland = std::pair<Type, Island>;

class AnalysisProfile
{
public:

    AnalysisProfile() = default;
    AnalysisProfile(int numEntry);
    AnalysisProfile(const AnalysisProfile& profile) = default;
    AnalysisProfile(AnalysisProfile&& profile) noexcept = default;

    ~AnalysisProfile() = default;

    AnalysisProfile& operator=(const AnalysisProfile& profile) = default;
    AnalysisProfile& operator=(AnalysisProfile&& profile) noexcept = default;

    int GetNumEntry() const;

    const std::vector<TypeIsland>& GetEntryTypeIsland(int num) const;

    void AddEntry(std::vector<TypeIsland> typeIs);

private:

    struct Entry
    {
        std::vector<TypeIsland> _typeIsland{ };
    };

    std::vector<Entry> _entry{ };
};

class MatchProfile
{
public:

    MatchProfile() = default;
    MatchProfile(const MatchProfile& profile) = default;
    MatchProfile(MatchProfile&& profile) noexcept = default;

    ~MatchProfile() = default;

    MatchProfile& operator=(const MatchProfile& profile) = default;
    MatchProfile& operator=(MatchProfile&& profile) noexcept = default;

    int GetNumEntry() const;

    Mode GetEntryMode(int num) const;
    const std::vector<TypeIdentity>& GetEntryTypeIdentity(int num) const;

    void AddEntry(Mode mode, std::vector<TypeIdentity> typeId);

private:

    struct Entry
    {
        Mode _mode{ Mode::COMPARE };
        std::vector<TypeIdentity> _typeIdentity{ };
    };

    std::vector<Entry> _entry{ };

    bool ValidateTypeIdentity(const std::vector<TypeIdentity>& typeId) const;
};

[[nodiscard]] AnalysisProfile Analyze(const std::vector<Square>& square);
[[nodiscard]] bool Match(const AnalysisProfile& analysis, const MatchProfile& match);

} // namespace Profiler

#endif // PROFILER_HPP
