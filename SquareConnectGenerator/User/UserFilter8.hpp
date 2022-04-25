/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#ifndef USER_FILTER_8
#define USER_FILTER_8

#include "Filter.hpp"
#include "Profiler.hpp"

#include <utility>

namespace UserFilter
{

void AddFilter8A(Filter& filter)
{
    // X, X, X, X, X, X, X, X,
    // X, O, O, O, O, O, D, X,
    // O, A, O, O, O, O, O, O,
    // O, X, X, X, X, O, O, O,
    // O, O, O, C, X, O, X, X,
    // O, X, X, O, O, O, B, O,
    // O, O, O, O, O, O, O, O,
    // O, O, O, O, X, O, O, O,

    Profiler::MatchProfile mat;
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::SINGLET_O,  Profiler::Identity::A   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::B   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::C   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::D   },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::SINGLET_O,  Profiler::Identity::A   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::B   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::C   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::D   },
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
            { Profiler::Type::SINGLET_O,  Profiler::Identity::A   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::B   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::C   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::D   },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::DUPLET_I,   Profiler::Identity::F   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::ANY },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::ANY },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::DUPLET_I,   Profiler::Identity::F   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::ANY },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::ANY },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::DUPLET_I,   Profiler::Identity::F   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::ANY },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::ANY },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::DUPLET_I,   Profiler::Identity::G   },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::ANY },
            { Profiler::Type::SINGLET_O,  Profiler::Identity::ANY },
        }
    );
    mat.AddEntry(
        Profiler::Mode::COMPARE,
        {
            { Profiler::Type::QUARTET_H,  Profiler::Identity::H   },
        }
    );

    filter.AddEntry(std::string("USER_FILTER_8_A"), 8, &mat);
}

void AddFilter8(Filter& filter)
{
    AddFilter8A(filter);
}

} // namespace UserFilter

#endif // USER_FILTER_8
