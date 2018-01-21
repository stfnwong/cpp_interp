/* SCANNER 
 * Scan over the source string and generate a list of 
 * Tokens.
 *
 * Stefan Wong 2018
 */

#include "Scanner.hpp"
#include "Token.hpp"

Scanner::Scanner(const std::string &source)
{
    this->source = source;
    this->start = 0;
    this->current = 0;
    this->line = 1;
}

bool Scanner::atEnd(void)
{
    return this->current >= this->source.size();
}

void Scanner::scanToken(void)
{
    char c = advance();
    switch(c)
    {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '-': addToken(MINUS); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case '*': addToken(STAR); break;
    }
}

std::list<Token> Scanner::ScanTokens(void)
{
    this->tokens = new std::list<Token>();

    while(!this->atEnd())
    {
        this->start = this->current;
        this->scanToken();
    }
    this->tokens.push_back(new Token(LOX_EOF, "", NULL, this->line));

    return tokens;
}
