/*
 * ERROR
 * Error handling utils
 */

#ifndef __ERROR_HPP
#define __ERROR_HPP

#include <string>


void Error(unsigned line, const std::string& msg);

void Report(unsigned line, const std::string& where, const std::string& msg);



#endif /*__ERROR_HPP*/
