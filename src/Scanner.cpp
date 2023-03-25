/* SCANNER 
 * Scan over the source string and generate a list of 
 * Tokens.
 *
 * Stefan Wong 2018
 */

#include "Scanner.hpp"


Scanner::Scanner(const std::string &source) : source(source), start(0), cur_pos(0), line(1) {}

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
        default:
            // TODO: report errors here
            break;
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
