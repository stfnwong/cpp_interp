/*
 * ERROR
 * Error handling utils
 */


#include <iostream>
#include "Error.hpp"

// TODO: when we add in the interpreter framework move this to the Loxpp class or
// whatever the top-level interpreter class becomes.

void Error(unsigned line, const std::string& msg)
{
    Report(line, "", msg);
}

void Report(unsigned line, const std::string& where, const std::string& msg)
{
    std::cout << "[line " << line << "] Error: " << where << ": " << msg << std::endl;
}
