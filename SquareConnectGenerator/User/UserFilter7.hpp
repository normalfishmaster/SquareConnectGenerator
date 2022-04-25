/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#ifndef USER_FILTER_7
#define USER_FILTER_7

#include "Filter.hpp"
#include "Profiler.hpp"

#include <utility>

namespace UserFilter
{

void AddFilter7A(Filter& filter)
{
    // O, O, O, O, X, O, O, O,
    // O, O, O, O, X, O, O, O,
    // O, O, O, C, O, O, O, O,
    // X, O, X, O, X, X, X, A,
    // X, O, X, O, X, X, X, D,
    // B, O, X, O, X, X, X, O,
    // O, O, O, O, O, O, O, O,
    // O, O, O, O, O, O, O, O,

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
            { Profiler::Type::TRIPLET_I,  Profiler::Identity::D   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::ANY },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::TRIPLET_I,  Profiler::Identity::D   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::ANY },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::DUPLET_I,   Profiler::Identity::E   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::ANY },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::ANY },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::DUPLET_I,   Profiler::Identity::E   },
            { Profiler::Type::DUPLET_I,   Profiler::Identity::F   },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::DUPLET_I,   Profiler::Identity::E   },
            { Profiler::Type::DUPLET_I,   Profiler::Identity::F   },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::DUPLET_I,   Profiler::Identity::E   },
            { Profiler::Type::DUPLET_I,   Profiler::Identity::F   },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::QUARTET_H,  Profiler::Identity::G   },
        }
    );

    filter.AddEntry(std::string("USER_FILTER_7_A"), 7, &mat);
}

void AddFilter7(Filter& filter)
{
    AddFilter7A(filter);
}

} // namespace UserFilter

#endif // USER_FILTER_7
