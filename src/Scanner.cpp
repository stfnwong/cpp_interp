/* SCANNER 
 * Scan over the source string and generate a list of 
 * Tokens.
 *
 * Stefan Wong 2018
 */

#include "Scanner.hpp"


Scanner::Scanner(const std::string &source) : source(source), start(0), cur_pos(0), line(1) {}

bool Scanner::at_end(void)
{
    return this->cur_pos >= this->source.size();
}

char Scanner::advance(void)
{
    // dont let the cur_pos go over the end of the source text array
    if(this->cur_pos >= this->source.size())
        this->cur_char = '\0';
    else
        this->cur_char = this->source[this->cur_pos];

    if(this->cur_char == '\n')
        this->line = this->line + 1;

    this->cur_pos++;
    return this->cur_char;
    //return this->source[this->cur_pos];
}



void Scanner::add_token(TokenType type)
{
    std::string lexeme = this->source.substr(this->start, this->cur_pos - this->start);
    this->tokens.push_back(Token(type, lexeme, this->line));
}

void Scanner::add_token(TokenType type, const std::string& literal)
{
    std::string lexeme = this->source.substr(this->start, this->cur_pos - this->start);
    Token new_token = Token(type, lexeme, this->line, literal);
    this->tokens.push_back(new_token);
}

void Scanner::add_token(TokenType type, float literal)
{
    std::string lexeme = this->source.substr(this->start, this->cur_pos - this->start);
    Token new_token = Token(type, lexeme, this->line, literal);
    this->tokens.push_back(new_token);
}

void Scanner::scan_token(void)
{
    char c = advance();
    
    switch(c)
    {
        case '(': add_token(TokenType::LEFT_PAREN); break;
        case ')': add_token(TokenType::RIGHT_PAREN); break;
        case '{': add_token(TokenType::LEFT_BRACE); break;
        case '}': add_token(TokenType::RIGHT_BRACE); break;
        case ',': add_token(TokenType::COMMA); break;
        case '.': add_token(TokenType::DOT); break;
        case '-': add_token(TokenType::MINUS); break;
        case '+': add_token(TokenType::PLUS); break;
        case ';': add_token(TokenType::SEMICOLON); break;
        case '*': add_token(TokenType::STAR); break;
        //default:

    }
}

std::vector<Token> Scanner::scan_tokens(void)
{
    this->tokens.clear();

    while(!this->at_end())
    {
        this->start = this->cur_pos;
        this->scan_token();
    }
    this->tokens.push_back(Token(TokenType::LOX_EOF, "", this->line));

    return tokens;
}
