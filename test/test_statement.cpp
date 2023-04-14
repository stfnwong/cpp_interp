/*
 * STATEMENT
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <list>

#include "Expr.hpp"
#include "Object.hpp"
#include "Statement.hpp"
#include "Token.hpp"


using T = LoxObject;
using E = LoxObject;


std::unique_ptr<Expr<E, T>> create_binary_expression(void)
{
    Token op(TokenType::PLUS, "+");
    
    std::unique_ptr<LiteralExpr<E, T>> left = std::make_unique<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 1.0)
    );

    // TODO: Bug - the object doesn't contain a token, so we can't detect errors in the
    // token itself. I have had the lexeme "1" here and it was not caught by the test.
    // Do I want to have a unique_ptr to a Token and do a move each time the token is 
    // moved?
    std::unique_ptr<LiteralExpr<E, T>> right = std::make_unique<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "2", 1, 2.0)
    );

    return std::make_unique<BinaryExpr<E, T>>(
            std::move(left),
            std::move(right),
            op
    );
}


TEST_CASE("test_create_print_statement", "stmt")
{
    std::string test_string = "print this";
    std::list<Token> test_tokens = {
        Token(TokenType::PRINT, "print", 1),
        Token(TokenType::STRING, test_string, 1, test_string)
    };


}

TEST_CASE("test_create_expression_statement", "stmt")
{
    std::unique_ptr<Expr<E, T>> test_expr = create_binary_expression();

    ExpressionStmt expr_stmt(std::move(test_expr));

    // I am downcasting the pointers by hand here to make it possible to write a single
    // REQUIRES() macro that uses the == operator. Its not suggested that this is good
    // practice in general.
    BinaryExpr<E, T>* derived_ptr = static_cast<BinaryExpr<E, T>*>(expr_stmt.expr.get());

    REQUIRE(derived_ptr->op == Token(TokenType::PLUS, "+"));
    REQUIRE(static_cast<LiteralExpr<E, T>*>(derived_ptr->left.get())->value == LoxObject(1.0f));
    REQUIRE(static_cast<LiteralExpr<E, T>*>(derived_ptr->right.get())->value == LoxObject(2.0f));
}


TEST_CASE("test_create_block_statement", "stmt")
{
    // Need to make a vector of Stmt pointers 
    std::vector<std::unique_ptr<Stmt<E, T>>> statements;

    unsigned num_stmt = 8;
    for(unsigned i = 0; i < num_stmt; ++i)
    {
        std::unique_ptr<Expr<E, T>> e = create_binary_expression();
        std::unique_ptr<Stmt<E, T>> s = std::make_unique<ExpressionStmt<E, T>>(std::move(e));
        statements.push_back(std::move(s));
    }

    std::string exp_string_repr = "ExpressionStmt<1 + 2>";
    for(unsigned i = 0; i < statements.size(); ++i)
        REQUIRE(statements[i]->to_string() == exp_string_repr);
}


//TEST_CASE("test_create_if_statement", "stmt")
//{
//}

