/*
 * UTIL
 * The stuff that doesn't go anywhere else\
 *
 */

#ifndef __UTIL_HPP
#define __UTIL_HPP

#include <cmath>


// Use default eps = 10e-6
inline bool float_equal(float a, float b)
{
    return (std::abs(a - b) < 10e-6) ? true : false;
}

inline bool float_equal_eps(float a, float b, float eps)
{
    return (std::abs(a - b) < eps) ? true : false;
}


#endif /*__UTIL_HPP*/
