/*
 * PARSER
 * A recursive descent parser
 */


#ifndef __PARSER_HPP
#define __PARSER_HPP


#include <exception>
#include <vector>

#include "Common.hpp"
#include "Expr.hpp"
#include "Statement.hpp"
#include "Token.hpp"




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

        // Expressions
        std::unique_ptr<Expr<ExprType, VisitType>> primary(void);            // literals, etc
        std::unique_ptr<Expr<ExprType, VisitType>> unary(void);
        std::unique_ptr<Expr<ExprType, VisitType>> factor(void);  // multiplication, division
        std::unique_ptr<Expr<ExprType, VisitType>> term(void);    // addition, subtraction
        std::unique_ptr<Expr<ExprType, VisitType>> equality(void);
        std::unique_ptr<Expr<ExprType, VisitType>> comparison(void);
        std::unique_ptr<Expr<ExprType, VisitType>> expression(void);

        // Statements 
        std::unique_ptr<Stmt<ExprType, VisitType>> declaration(void);            // literals, etc
        std::unique_ptr<Stmt<ExprType, VisitType>> var_declaration(void);            // literals, etc
        std::unique_ptr<Stmt<ExprType, VisitType>> statement(void);
        std::unique_ptr<Stmt<ExprType, VisitType>> print_statement(void);
        std::unique_ptr<Stmt<ExprType, VisitType>> expression_statement(void);

    public:
        Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}
        std::vector<std::unique_ptr<Stmt<ExprType, VisitType>>> parse(void);
        //std::unique_ptr<Expr<ExprType, VisitType>> parse(void);
        unsigned num_tokens(void) const;
};


#endif /*__PARSER_HPP*/
