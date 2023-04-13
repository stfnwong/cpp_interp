/*
 * PARSER
 * A recursive descent parser
 */

#include <iostream>    // TODO: debug only, remove

#include "Error.hpp"
#include "Lox.hpp"
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
    // TODO: some copy ctor issue with token...
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

std::unique_ptr<Expr<EType, VType>> Parser::primary(void)
{
    //if(this->match({TokenType::FALSE}))
    //    return std::make_unique<LiteralExpr<EType, VType>>(this->peek());

    if(this->match({TokenType::TRUE, TokenType::FALSE, TokenType::NIL}))
        return std::make_unique<LiteralExpr<EType, VType>>(this->previous());

    if(this->match({TokenType::STRING, TokenType::NUMBER}))
    {
        Token prev = this->previous();
        auto ret = std::make_unique<LiteralExpr<EType, VType>>(prev);
        return ret;
    }

    if(this->match({TokenType::IDENTIFIER}))
        return std::make_unique<VariableExpr<EType, VType>>(this->previous());

    if(this->match({TokenType::LEFT_PAREN}))
    {
        auto expr = this->expression();
        this->consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return std::make_unique<GroupingExpr<EType, VType>>(std::move(expr));
    }

    Lox::error(this->peek(), "expected expression");
    throw ParseError(this->peek(), "expected expression");
}


std::unique_ptr<Expr<EType, VType>> Parser::unary(void)
{
    if(this->match({TokenType::BANG, TokenType::MINUS}))
    {
        Token op = this->previous();
        auto right = this->unary();
        return std::make_unique<UnaryExpr<EType, VType>>(std::move(right), op);
    }

    return this->primary();
}

std::unique_ptr<Expr<EType, VType>> Parser::factor(void)
{
    auto expr = this->unary();

    while(this->match({TokenType::SLASH, TokenType::STAR}))
    {
        Token op = this->previous();
        auto right = this->unary();
        expr = std::make_unique<BinaryExpr<EType, VType>>(
                std::move(expr), std::move(right), op
        );
    }

    return expr;
}

std::unique_ptr<Expr<EType, VType>> Parser::term(void)
{
    auto expr = this->factor();
    
    while(this->match({TokenType::MINUS, TokenType::PLUS}))
    {
        Token op = this->previous();
        std::unique_ptr<Expr<EType, VType>> right = this->factor();
        expr = std::make_unique<BinaryExpr<EType, VType>>(
                std::move(expr), std::move(right), op
        );
    }

    return expr;
}

std::unique_ptr<Expr<EType, VType>> Parser::comparison(void)
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
        expr = std::make_unique<BinaryExpr<EType, VType>>(
                std::move(expr), std::move(right), op
        );
    }

    return expr;
}

std::unique_ptr<Expr<EType, VType>> Parser::equality(void) 
{
    auto expr = this->comparison();

    while(this->match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL}))
    {
        Token op = this->previous();
        auto right = this->comparison();
        expr = std::make_unique<BinaryExpr<EType, VType>>(std::move(expr), std::move(right), op);
    }

    return expr;
}

std::unique_ptr<Expr<EType, VType>> Parser::expression(void)
{
    return this->assignment();
    //return this->equality();
}

std::unique_ptr<Expr<EType, VType>> Parser::assignment(void)
{
    std::unique_ptr<Expr<EType, VType>> expr = this->equality();

    if(this->match({TokenType::EQUAL}))
    {
        Token equals = this->previous();
        std::unique_ptr<Expr<EType, VType>> value = this->assignment();

        if(value->get_type() == ExprType::VARIABLE)
        {
            Token name = static_cast<VariableExpr<EType, VType>*>(value.get())->token;
            return std::make_unique<AssignmentExpr<EType, VType>>(name, std::move(expr));
        }

        Lox::error(equals, "invalid assignment target");
    }

    return expr;
}

// ======== STATEMENT  FUNCTIONS ======== //
std::unique_ptr<Stmt<EType, VType>> Parser::declaration(void)
{
    try {
        if(this->match({TokenType::VAR}))
            return this->var_declaration();

        return this->statement();
    }
    catch(ParseError& e)
    {
        this->synchronise();
        return nullptr;
    }
}


std::unique_ptr<Stmt<EType, VType>> Parser::var_declaration(void)
{
    Token name = this->consume(TokenType::IDENTIFIER, "expect variable name");

    std::unique_ptr<Expr<EType, VType>> initialiser = nullptr;
    if(this->match({TokenType::EQUAL}))
        initialiser = this->expression();

    this->consume(TokenType::SEMICOLON, "expect ';' after variable declaration");

    return std::make_unique<VariableStmt<EType, VType>>(name, std::move(initialiser));
}

std::unique_ptr<Stmt<EType, VType>> Parser::statement(void)
{
    if(this->match({TokenType::PRINT}))
        return this->print_statement();

    return this->expression_statement();
}


std::unique_ptr<Stmt<EType, VType>> Parser::print_statement(void)
{
    auto value = this->expression();
    this->consume(TokenType::SEMICOLON, "expect ';' after value");
    return std::make_unique<PrintStmt<EType, VType>>(std::move(value));
}

std::unique_ptr<Stmt<EType, VType>> Parser::expression_statement(void)
{
    auto value = this->expression();
    this->consume(TokenType::SEMICOLON, "expect ';' after expression");
    return std::make_unique<ExpressionStmt<EType, VType>>(std::move(value));
}


// ======== PUBLIC  FUNCTIONS ======== //
std::vector<std::unique_ptr<Stmt<EType, VType>>> Parser::parse(void)
{
    using StmtPtr = std::unique_ptr<Stmt<EType, VType>>;
    std::vector<StmtPtr> statements; 

    if(this->tokens.size() == 0)
        return statements;

    try 
    {
        while(!this->at_end())
            statements.push_back(std::move(this->declaration()));
    }
    catch(ParseError& error)
    {
        this->synchronise();
    }

    return statements;
}

unsigned Parser::num_tokens(void) const {
    return this->tokens.size();
}
