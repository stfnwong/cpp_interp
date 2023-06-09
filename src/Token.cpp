/* TOKEN 
 * Token definitions 
 *
 * Stefan Wong 2018
 */

#include <string>
#include <sstream>
#include <vector>

#include "Token.hpp"


std::unordered_map<TokenType, std::string> token_to_str = {
    {TokenType::LEFT_PAREN, "LEFT_PAREN"}, 
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"}, 
    {TokenType::LEFT_BRACE, "LEFT_BRACE"}, 
    {TokenType::RIGHT_BRACE, "RIGHT_BRACE"}, 
    {TokenType::COMMA, "COMMA"}, 
    {TokenType::DOT, "DOT"}, 
    {TokenType::MINUS, "MINUS"}, 
    {TokenType::PLUS, "PLUS"}, 
    {TokenType::SEMICOLON, "SEMICOLON"}, 
    {TokenType::SLASH, "SLASH"}, 
    {TokenType::STAR, "STAR"},
    // 1-2 character tokens 
    {TokenType::BANG, "BANG"}, {TokenType::BANG_EQUAL, "BANG_EQUAL"}, 
    {TokenType::EQUAL, "EQUAL"}, {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
    {TokenType::GREATER, "GREATER"}, {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
    {TokenType::LESS, "LESS"}, {TokenType::LESS_EQUAL, "LESS_EQUAL"},
    // Literals
    {TokenType::IDENTIFIER, "IDENTIFIER"}, {TokenType::STRING, "STRING"}, 
    {TokenType::NUMBER, "NUMBER"},
    // Keywords 
    {TokenType::AND, "AND"}, {TokenType::CLASS, "CLASS"}, {TokenType::ELSE, "ELSE"}, 
    {TokenType::FALSE, "FALSE"}, {TokenType::FUN, "FUN"}, {TokenType::FOR, "FOR"}, 
    {TokenType::IF, "IF"}, {TokenType::NIL, "NIL"}, {TokenType::OR, "OR"},
    {TokenType::PRINT, "PRINT"}, {TokenType::RETURN, "RETURN"}, 
    {TokenType::SUPER, "SUPER"}, {TokenType::THIS, "THIS"}, {TokenType::TRUE, "TRUE"}, 
    {TokenType::VAR, "VAR"}, {TokenType::WHILE, "WHILE"},
    // End of file 
    {TokenType::LOX_EOF, "LOX_EOF"},
    {TokenType::EMPTY, "EMPTY"}
};


// ======== Operators ========
bool Token::operator==(const Token& that) const
{
    if(this->type != that.type)
        return false;
    if(this->lexeme != that.lexeme)
        return false;
    if(this->line != that.line)
        return false;
    // TODO: how to test doubles? We want to call 
    // double_equal(this->literal.value(), that.literal.value()) here if variant is a double
    if(this->literal.has_value() && that.literal.has_value())
        return (this->literal.value() == that.literal.value()) ? true : false;

    return true;
}

bool Token::operator!=(const Token& that) const
{
    return !(*this == that);
}

Token& Token::operator=(Token that) 
{
    swap(*this, that);
    return *this;
}



std::string Token::get_string_literal(void) const
{
    if(this->literal.has_value())
        return std::get<std::string>(this->literal.value());

    return {};
}

double Token::get_double_literal(void) const
{
    if(this->literal.has_value())
        return std::get<double>(this->literal.value());

    return {};
}


bool Token::has_literal(void) const
{
    return this->literal.has_value();
}

std::string Token::to_string(void) const
{
    //if(this->literal.has_value())
    //    return std::visit(make_literal_string(), this->literal.value());

    return this->lexeme;
}

std::string Token::to_repr(void) const
{
    std::ostringstream oss;

    // TODO: move make_literal_string inside class?
    oss << "Token<" << token_to_str[this->type] << "> \"" << this->lexeme << "\"";
    if(this->literal.has_value())
    {
        oss << " [" << std::visit(make_literal_string(), this->literal.value()) << "] ";
    }
    oss << " (line: " << this->line << ") ";

    return oss.str();
}

