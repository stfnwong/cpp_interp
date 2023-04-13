/* SCANNER 
 * Scan over the source string and generate a list of 
 * Tokens.
 *
 * Stefan Wong 2018
 */

// TODO: possibly remove this...
#include <cctype>
#include <iostream>

#include "Lox.hpp"
#include "Scanner.hpp"


Scanner::Scanner(const std::string &source) : source(source), start(0), cur_pos(0), line(1) 
{
    this->add_keywords();
}

void Scanner::add_keywords(void)
{
    this->keywords["and"] = TokenType::AND;
    this->keywords["class"] = TokenType::CLASS;
    this->keywords["else"] = TokenType::ELSE;
    this->keywords["false"] = TokenType::FALSE;
    this->keywords["fun"] = TokenType::FUN;
    this->keywords["for"] = TokenType::FOR;
    this->keywords["if"] = TokenType::IF;
    this->keywords["nil"] = TokenType::NIL;
    this->keywords["or"] = TokenType::OR;
    this->keywords["print"] = TokenType::PRINT;
    this->keywords["return"] = TokenType::RETURN;
    this->keywords["super"] = TokenType::SUPER;
    this->keywords["this"] = TokenType::THIS;
    this->keywords["true"] = TokenType::TRUE;
    this->keywords["var"] = TokenType::VAR;
    this->keywords["while"] = TokenType::WHILE;
}

bool Scanner::at_end(void) const
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
    //return this->cur_char;
    return this->source[this->cur_pos-1];
}

bool Scanner::match(char exp) 
{
    if(this->at_end())
        return false;
    if(this->source[this->cur_pos] != exp)
        return false;

    this->cur_pos++;
    return true;
}

char Scanner::peek(void) const
{
    if(this->at_end())
        return '\0';
    return this->source[this->cur_pos];
}

char Scanner::peek_next(void) const
{
    if(this->cur_pos + 1 >= this->source.length())
        return '\0';

    return this->source[this->cur_pos+1];
}

bool Scanner::is_alphanum(char c) const
{
    return (std::isalnum(c) || c == '_') ? true : false;
}

void Scanner::add_token(TokenType type)
{
    std::string lexeme = this->get_cur_lexeme();
    this->tokens.push_back(Token(type, lexeme, this->line));
}

void Scanner::add_token(TokenType type, const std::string& literal)
{
    std::string lexeme = this->get_cur_lexeme();
    Token new_token = Token(type, lexeme, this->line, literal);
    this->tokens.push_back(new_token);
}

void Scanner::add_token(TokenType type, float literal)
{
    std::string lexeme = this->get_cur_lexeme();
    Token new_token = Token(type, lexeme, this->line, literal);
    this->tokens.push_back(new_token);
}

void Scanner::scan_token(void)
{
    char c = advance();
    
    switch(c)
    {
        // one character tokens
        case '(': this->add_token(TokenType::LEFT_PAREN); break;
        case ')': this->add_token(TokenType::RIGHT_PAREN); break;
        case '{': this->add_token(TokenType::LEFT_BRACE); break;
        case '}': this->add_token(TokenType::RIGHT_BRACE); break;
        case ',': this->add_token(TokenType::COMMA); break;
        case '.': this->add_token(TokenType::DOT); break;
        case '-': this->add_token(TokenType::MINUS); break;
        case '+': this->add_token(TokenType::PLUS); break;
        case ';': this->add_token(TokenType::SEMICOLON); break;
        case '*': this->add_token(TokenType::STAR); break;

        // two character tokens 
        case '!': this->add_token(this->match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '=': this->add_token(this->match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
        case '<': this->add_token(this->match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
        case '>': this->add_token(this->match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;

        case '/':
            if(this->match('/'))
            {
                while(this->peek() != '\n' && !this->at_end())
                    this->advance();
            }
            else
                this->add_token(TokenType::SLASH);
            break;

        case ' ':
        case '\r':
        case '\t':
            // ignore whitespace
            break;

        case '\n':
            this->line++;
            break;

        // handle string literals
        case '"':
            this->string_literal();
            break;

        default:
            if(std::isdigit(c))
                this->number_literal();
            else if(this->is_alphanum(c))
                this->identifier();
            else
                Lox::error(this->line, "Unknown token"); // TODO: format a string here
            break;
    }
}

std::string Scanner::get_cur_lexeme(void)
{
    return this->source.substr(this->start, this->cur_pos - this->start);
}

void Scanner::string_literal(void)
{
    while(this->peek() != '"' && !this->at_end())
    {
        if(this->peek() == '\n')
            this->line++;
        this->advance();
    }

    if(this->at_end())
    {
        Lox::error(this->line, "unterminated string");
        return;
    }

    // handle closing '"'
    this->advance();
    std::string lexeme = this->source.substr(this->start+1, this->cur_pos - this->start - 2);
    this->tokens.push_back(
            Token(TokenType::STRING, lexeme, this->line)
    );
}

void Scanner::number_literal(void)
{
    while(std::isdigit(this->peek()))
        this->advance();

    // Look for a fractional part
    if(this->peek() == '.' && std::isdigit(this->peek_next()))
        this->advance();        // skip over '.'
    while(std::isdigit(this->peek()))
        this->advance();

    std::string lexeme = this->get_cur_lexeme();
    Token num_token = Token(TokenType::NUMBER, lexeme, this->line, std::stof(lexeme));
    this->tokens.push_back(num_token);
}

void Scanner::identifier(void)
{
    while(this->is_alphanum(this->peek()))
        this->advance();

    // Check if this is a known keyword
    std::string lexeme = this->get_cur_lexeme();

    Token iden_token;
    if(this->keywords.find(lexeme) != this->keywords.end())
        iden_token = Token(this->keywords[lexeme], lexeme, this->line);
    else
        iden_token = Token(TokenType::IDENTIFIER, lexeme, this->line);
    this->tokens.push_back(iden_token);
}

std::vector<Token> Scanner::scan(void)
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
