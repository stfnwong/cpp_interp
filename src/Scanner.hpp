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
#include <unordered_map>

#include "Token.hpp"

constexpr const unsigned token_buf_size = 256;


class Scanner
{
    private:

        std::unordered_map<std::string, TokenType> keywords;

        std::string source;
        std::vector<Token> tokens;

        // populate keyword map
        void add_keywords(void);

        // keep track of position in source 
        unsigned int start;
        unsigned int cur_pos;
        unsigned int line;
        char cur_char;
        std::array<char, token_buf_size> token_buf;

        // Check if we have consumed all input
        bool at_end(void) const;
        // Token scan helper functions 
        char advance(void);
        bool match(char exp);
        char peek(void) const;
        char peek_next(void) const;
        bool is_alphanum(char c) const;
        void add_token(TokenType type);
        void add_token(TokenType type, const std::string& literal);
        void add_token(TokenType type, float literal);

        std::string get_cur_lexeme(void);
        void string_literal(void);
        void number_literal(void);
        void identifier(void);
        // Scan the next token in sequence 
        void scan_token(void);

    public:
        Scanner(const std::string &source);
        ~Scanner() {} 
        std::vector<Token> scan(void);
};




#endif /*__SCANNER_HPP*/
