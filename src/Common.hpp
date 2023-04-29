/*
 * COMMON
 * Typedefs, etc, here
 *
 */

#ifndef __COMMON_HPP
#define __COMMON_HPP


#include "Object.hpp"

using EType = LoxObject;  // Type of data in expression
using VType = LoxObject; // Type of data returned by ExprVisitor
using StmtVType = LoxObject; // Type of data returned by StmtVisitor
                             //
const constexpr unsigned MAX_ARGUMENTS = 255;


#endif /*__COMMON_HPP*/
