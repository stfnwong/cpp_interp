/* TOKEN 
 * Token definitions 
 *
 * Stefan Wong 2018
 */

#include <string>
#include "Token.hpp"

Token::Token(TokenType type, const std::string &lexeme, void* literal, int line)
{
    this->token = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
}


Token::~Token() {} 


std::string Token::toString(void)
{
    std::string tok_str;

    tok_str << this->type << " " << this->lexeme << " " << this->literal;
    eturn tok_str;
}
