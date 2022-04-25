/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#ifndef USER_FILTER_6
#define USER_FILTER_6

#include "Filter.hpp"
#include "Profiler.hpp"

#include <utility>

namespace UserFilter
{

void AddFilter6A(Filter& filter)
{
    // O, O, O, O, O, O, O, O,
    // X, O, C, X, X, X, O, O,
    // O, O, O, O, O, O, O, O,
    // O, O, X, X, X, X, A, X,
    // O, O, X, X, X, X, D, X,
    // O, O, O, B, X, X, O, X,
    // O, O, X, X, X, O, O, X,
    // O, O, X, X, X, O, O, X,

    Profiler::MatchProfile mat;
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::DUPLET_I,   Profiler::Identity::A   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::B   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::C   },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::DUPLET_I,   Profiler::Identity::A   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::B   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::C   },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::DUPLET_I,   Profiler::Identity::A   },
            { Profiler::Type::DUPLET_I,   Profiler::Identity::D   },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::DUPLET_I,   Profiler::Identity::A   },
            { Profiler::Type::DUPLET_I,   Profiler::Identity::D   },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::DUPLET_I,   Profiler::Identity::A   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::B   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::C   },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::DUPLET_I,   Profiler::Identity::A   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::B   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::C   },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::QUARTET_H,  Profiler::Identity::E   },
        }
    );

    filter.AddEntry(std::string("USER_FILTER_6_A"), 6, &mat);
}


void AddFilter6(Filter& filter)
{
    AddFilter6A(filter);
}

} // namespace UserFilter

#endif // USER_FILTER_6
