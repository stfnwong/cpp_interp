/*
 * UTIL
 * The stuff that doesn't go anywhere else
 */

#ifndef __UTIL_HPP
#define __UTIL_HPP

#include <cmath>
#include <string>


constexpr const bool EPS = 1e-6;

// Use default eps = 10e-6
inline bool float_equal(float a, float b)
{
    return (std::fabs(a - b) < EPS) ? true : false;
}

inline bool float_equal_eps(float a, float b, float eps)
{
    return (std::fabs(a - b) < eps) ? true : false;
}

inline bool double_equal(double a, double b)
{
    return (std::fabs(a - b) < EPS) ? true : false;
}

inline bool double_equal_eps(double a, double b, double eps)
{
    return (std::fabs(a - b) < eps) ? true : false;
}


// File handling 
std::string read_file(const std::string& filename);


#endif /*__UTIL_HPP*/
