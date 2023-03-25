/* SCANNER 
 * Scan over the source string and generate a list of 
 * Tokens.
 *
 * Stefan Wong 2018
 */

#ifndef __SCANNER_HPP
#define __SCANNER_HPP

#include <array>
#include <string>
#include <variant>
#include <vector>

#include "Token.hpp"

constexpr const unsigned token_buf_size = 256;


class Scanner
{
    private:

        std::string source;
        std::vector<Token> tokens;

        // keep track of position in source 
        unsigned int start;
        unsigned int cur_pos;
        unsigned int line;
        char cur_char;
        std::array<char, token_buf_size> token_buf;

        // Check if we have consumed all input
        bool at_end(void);
        // Token scan helper functions 
        char advance(void);
        void add_token(TokenType type);
        void add_token(TokenType type, const std::string& literal);
        void add_token(TokenType type, float literal);
        // Scan the next token in sequence 
        void scan_token(void);

    public:
        Scanner(const std::string &source);
        ~Scanner() {} 
        std::vector<Token> scan_tokens(void);
};




#endif /*__SCANNER_HPP*/
