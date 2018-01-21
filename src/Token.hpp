/* TOKEN 
 * Token definitions 
 *
 * Stefan Wong 2018
 */

#ifndef __TOKEN_HPP
#define __TOKEN_HPP

#include <string>

enum class TokenType{
    // Single character tokens 
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, 
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    // 1-2 character tokens 
    BANG, BANG_EQUAL, 
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    // Literals
    IDENTIFIER, STRING, NUMBER,
    // Keywords 
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
    // End of file 
    LOX_EOF
};

class Token
{
    protected:
        TokenType token;
        std::string lexeme;
        int line;
        void *literal;

    public: 
        Token(TokenType type, const std::string &lexeme, void* literal, int line);     // TODO : Object literal
        ~Token();
        std::string toString(void);
};




#endif /*__TOKEN_HPP*/
