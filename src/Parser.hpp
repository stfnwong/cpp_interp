/*
 * PARSER
 * A recursive descent parser
 */


#ifndef __PARSER_HPP
#define __PARSER_HPP


#include <exception>
#include <vector>

#include "Expr.hpp"
#include "Token.hpp"


struct ParseError : public std::exception
{
    Token token;
    std::string msg_;

    public:
        explicit ParseError(const Token& tok, const std::string& msg) : token(tok), msg_(msg) {} 
};


using T = std::string;
using E = LoxObject;


class Parser
{
    std::vector<Token> tokens;
    int current;

    private:
        bool  at_end(void) const;
        bool  check(const TokenType& t);
        bool  match(const std::vector<TokenType>& types);
        Token peek(void) const;
        Token previous(void) const;
        Token advance(void);
        Token consume(const TokenType& type, const std::string& msg);

        void synchronise(void);

        std::unique_ptr<Expr<E, T>> primary(void);            // literals, etc
        std::unique_ptr<Expr<E, T>> unary(void);
        std::unique_ptr<Expr<E, T>> factor(void);  // multiplication, division
        std::unique_ptr<Expr<E, T>> term(void);    // addition, subtraction
        std::unique_ptr<Expr<E, T>> equality(void);
        std::unique_ptr<Expr<E, T>> comparison(void);
        std::unique_ptr<Expr<E, T>> expression(void);

    public:
        Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}
        std::unique_ptr<Expr<E, T>> parse(void);
        unsigned num_tokens(void) const;
};


#endif /*__PARSER_HPP*/
