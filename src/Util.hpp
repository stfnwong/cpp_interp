/*
 * UTIL
 * The stuff that doesn't go anywhere else\
 *
 */

#ifndef __UTIL_HPP
#define __UTIL_HPP

#include <cmath>

inline bool float_equal(float a, float b, float eps)
{
    return (std::abs(a - b) < eps) ? true : false;
}


#endif /*__UTIL_HPP*/
