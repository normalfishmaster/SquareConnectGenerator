/* Copyright (C) Normal Fish Studios - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Dennis Law <normalfish.master@gmail.com>, April 2022
 */

#ifndef USER_FILTER_HPP
#define USER_FILTER_HPP

#include "UserFilter6.hpp"
#include "UserFilter7.hpp"
#include "UserFilter8.hpp"

#include "Filter.hpp"

namespace UserFilter
{

void AddEntries(Filter& filter)
{
    AddFilter6(filter);
    AddFilter7(filter);
    AddFilter8(filter);
}

} // namespace UserFilter

#endif // USER_FILTER_HPP
