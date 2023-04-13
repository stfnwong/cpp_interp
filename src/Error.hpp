/*
 * 
 * ERROR
 * Error handling utils
 */

#ifndef __ERROR_HPP
#define __ERROR_HPP

#include <string>

#include "Token.hpp"



struct ParseError : public std::exception
{
    Token token;
    std::string msg_;

    public:
        explicit ParseError(const Token& tok, const std::string& msg) : token(tok), msg_(msg) {} 
};


struct RuntimeError : public std::exception
{
    Token token;
    std::string msg_;

    public:
        explicit RuntimeError(const Token& tok, const std::string& msg) : 
            token(tok), 
            msg_(msg) {} 

        const char* what() {
            return this->msg_.c_str();
        }
};



#endif /*__ERROR_HPP*/
