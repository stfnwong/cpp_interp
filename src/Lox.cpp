/*
 * LOX
 * Global interpreter state (eg: error reporting)
 */

#include <iostream>
#include "Lox.hpp"


bool Lox::had_error = false;
bool Lox::had_runtime_error = false;


void Lox::report(int line, const std::string& where, const std::string& msg)
{
    std::cerr << "[line " << line << "] Error : " << where
        << ": " << msg << std::endl;
}

void Lox::error(int line, const std::string& msg)
{
    report(line, "", msg);
    Lox::had_error = true;
}

void Lox::error(const Token& tok, const std::string& msg)
{
    if(tok.type == TokenType::LOX_EOF)
        report(tok.line, " at end ", msg);
    else 
        report(tok.line, " at " + tok.lexeme + " ", msg);
    Lox::had_error = true;
}

