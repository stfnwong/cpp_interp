/*
 * LOX
 * Global interpreter state (eg: error reporting)
 */

#ifndef __LOX_HPP
#define __LOX_HPP


#include <string>

#include "Token.hpp"


// In the original book all the error handling stuff is placed into a static 
// class.


struct Lox
{
    public:
        static void report(int line, const std::string& where, const std::string& msg);
        static void error(int line, const std::string& msg);
        static void error(const Token& tok, const std::string& msg);


        static bool had_error;
        static bool had_runtime_error;
    };


#endif /*__LOX_HPP*/
