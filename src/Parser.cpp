/*
 * PARSER
 * A recursive descent parser
 */

#include <iostream>

#include "Parser.hpp"

bool Parser::at_end(void) const
{
    return this->peek().type == TokenType::LOX_EOF;
    //return this->current >= this->tokens.size()-1 ? true : false;
}

bool Parser::check(const TokenType& t)
{
    if(this->at_end())
        return false;

    return this->peek().type == t;
}

bool Parser::match(const std::vector<TokenType>& types)
{
    for(const TokenType& t : types)
    {
        if(this->check(t))
        {
            this->advance();
            return true;
        }
    }

    return false;
}

Token Parser::peek(void) const
{
    return this->tokens[this->current];
}

Token Parser::previous(void) const
{
    return this->tokens[this->current-1];
}

Token Parser::advance(void)
{
    if(!this->at_end())
        this->current++;

    return this->previous();
}

Token Parser::consume(const TokenType& t, const std::string& msg)
{
    if(this->check(t))
        return this->advance();

    throw ParseError(this->peek(), msg);
}

void Parser::synchronise(void)
{
    this->advance();

    while(!this->at_end())
    {
        // Statement is definitely terminated
        if(this->previous().type == TokenType::SEMICOLON)
            return;

        switch(this->peek().type)
        {
            case TokenType::CLASS:
            case TokenType::FUN:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
            // shut linter up
            default:
                continue;
        }
    }

    this->advance();
}


// ======== GRAMMAR RULES ======== //
std::unique_ptr<Expr<E, T>> Parser::primary(void)
{
    //if(this->match({TokenType::FALSE}))
    //    return std::make_unique<LiteralExpr<E, T>>(this->peek());

    if(this->match({TokenType::TRUE, TokenType::FALSE, TokenType::NIL}))
        return std::make_unique<LiteralExpr<E, T>>(this->previous());

    if(this->match({TokenType::STRING, TokenType::NUMBER}))
        return std::make_unique<LiteralExpr<E, T>>(this->previous());

    if(this->match({TokenType::LEFT_PAREN}))
    {
        auto expr = this->expression();
        this->consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return std::make_unique<GroupingExpr<E, T>>(std::move(expr));
    }

    throw ParseError(this->peek(), "Expected expression");
}

std::unique_ptr<Expr<E, T>> Parser::unary(void)
{
    if(this->match({TokenType::BANG, TokenType::MINUS}))
    {
        Token op = this->previous();
        auto right = this->unary();
        return std::make_unique<UnaryExpr<E, T>>(std::move(right), op);
    }

    return this->primary();
}

std::unique_ptr<Expr<E, T>> Parser::factor(void)
{
    auto expr = this->unary();

    while(this->match({TokenType::SLASH, TokenType::STAR}))
    {
        Token op = this->previous();
        auto right = this->unary();
        expr = std::make_unique<BinaryExpr<E, T>>(
                std::move(expr), std::move(right), op
        );
    }

    return expr;
}

std::unique_ptr<Expr<E, T>> Parser::term(void)
{
    auto expr = this->factor();
    
    while(this->match({TokenType::MINUS, TokenType::PLUS}))
    {
        Token op = this->previous();
        std::unique_ptr<Expr<E, T>> right = this->factor();
        expr = std::make_unique<BinaryExpr<E, T>>(
                std::move(expr), std::move(right), op
        );
    }

    return expr;
}

std::unique_ptr<Expr<E, T>> Parser::comparison(void)
{
    auto expr = this->term();

    while(this->match({
                TokenType::GREATER,
                TokenType::GREATER_EQUAL,
                TokenType::LESS,
                TokenType::LESS_EQUAL}))
    {
        Token op = this->previous();
        auto right = this->term();
        expr = std::make_unique<BinaryExpr<E, T>>(
                std::move(expr), std::move(right), op
        );
    }

    return expr;
}

std::unique_ptr<Expr<E, T>> Parser::equality(void) 
{
    auto expr = this->comparison();

    while(this->match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {
        Token op = this->previous();
        auto right = this->comparison();
        expr = std::make_unique<BinaryExpr<E, T>>(std::move(expr), std::move(right), op);
    }

    return expr;
}

std::unique_ptr<Expr<E, T>> Parser::expression(void)
{
    return this->equality();
}

// ======== STATEMENT  FUNCTIONS ======== //
std::unique_ptr<Stmt<E, T>> Parser::statement(void)
{
    if(this->match({TokenType::PRINT}))
        return this->print_statement();

    return this->expression_statement();
}


std::unique_ptr<Stmt<E, T>> Parser::print_statement(void)
{
    auto value = this->expression();
    this->consume(TokenType::SEMICOLON, "Expect ';' after value");
    return nullptr;
    //return std::make_unique<PrintStmt<E, T>>(value);
}

std::unique_ptr<Stmt<E, T>> Parser::expression_statement(void)
{
    auto value = this->expression();
    this->consume(TokenType::SEMICOLON, "Expect ';' after expression");
    return nullptr;
    //return std::make_unique<ExpressionStmt<E, T>>(std::move(value));
}


// ======== PUBLIC  FUNCTIONS ======== //
std::list<std::unique_ptr<Stmt<E, T>>> Parser::parse(void)
{
    using StmtPtr = std::unique_ptr<Stmt<E, T>>;
    if(this->tokens.size() > 0)
    {
        std::list<StmtPtr> statements; 
        while(!this->at_end())
            statements.push_back(this->statement());
    }
    else
        throw ParseError(Token(), "Got 0 input tokens");
}

unsigned Parser::num_tokens(void) const {
    return this->tokens.size();
}
