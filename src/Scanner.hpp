/* SCANNER 
 * Scan over the source string and generate a list of 
 * Tokens.
 *
 * Stefan Wong 2018
 */

#ifndef __SCANNER_HPP
#define __SCANNER_HPP

#include <string>
#include <list>
#include "Token.hpp"

class Scanner
{
    protected:
        std::string source;
        std::list<Token> tokens;
        // keep track of position in source 
        unsigned int start;
        unsigned int current;
        unsigned int line;
        // Check if we have consumed all input
        bool atEnd(void);
        // Token scan helper functions 
        char advance(void);
        void addToken(TokenType type);
        void addToken(TokenType type, void *literal);
        // Scan the next token in sequence 
        void scanToken(void);

    public:
        Scanner(const std::string &source);
        ~Scanner();
        std::list<Token> ScanTokens(void);
};




#endif /*__SCANNER_HPP*/
