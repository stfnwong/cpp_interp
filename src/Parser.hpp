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

        void  synchronise(void);
        std::unique_ptr<Expr<EType, VType>> finish_call(std::unique_ptr<Expr<EType, VType>> callee);

        // Expressions
        std::unique_ptr<Expr<EType, VType>> primary(void);            // literals, etc
        std::unique_ptr<Expr<EType, VType>> call(void);
        std::unique_ptr<Expr<EType, VType>> unary(void);
        std::unique_ptr<Expr<EType, VType>> factor(void);  // multiplication, division
        std::unique_ptr<Expr<EType, VType>> term(void);    // addition, subtraction
        std::unique_ptr<Expr<EType, VType>> equality(void);
        std::unique_ptr<Expr<EType, VType>> comparison(void);
        std::unique_ptr<Expr<EType, VType>> expression(void);
        std::unique_ptr<Expr<EType, VType>> assignment(void);
        std::unique_ptr<Expr<EType, VType>> or_expr(void);
        std::unique_ptr<Expr<EType, VType>> and_expr(void);

        // Statements 
        std::unique_ptr<Stmt<EType, VType>> declaration(void);            // literals, etc
        std::unique_ptr<Stmt<EType, VType>> func(const std::string& kind);            // literals, etc
        std::unique_ptr<Stmt<EType, VType>> var_declaration(void);            // literals, etc
        std::unique_ptr<Stmt<EType, VType>> statement(void);
        std::unique_ptr<Stmt<EType, VType>> if_statement(void);
        std::unique_ptr<Stmt<EType, VType>> for_statement(void);
        std::unique_ptr<Stmt<EType, VType>> while_statement(void);
        std::unique_ptr<Stmt<EType, VType>> print_statement(void);
        std::unique_ptr<Stmt<EType, VType>> return_statement(void);
        std::unique_ptr<Stmt<EType, VType>> expression_statement(void);
        std::unique_ptr<Stmt<EType, VType>> block(void);

    public:
        Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}
        std::vector<std::unique_ptr<Stmt<EType, VType>>> parse(void);
        unsigned num_tokens(void) const;
};


#endif /*__PARSER_HPP*/
