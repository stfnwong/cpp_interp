/*
 * EXPRESSIONS
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"


#include "Expr.hpp"
#include "Token.hpp"


TEST_CASE("test_create_binary_expr", "expression")
{
    std::shared_ptr<LiteralExpr> left = std::make_shared<LiteralExpr>(
            LoxObject(
                Token(TokenType::NUMBER, "1", 1, 1.0)
            )
        );

    std::shared_ptr<LiteralExpr>  right = std::make_shared<LiteralExpr>(
            LoxObject(
                Token(TokenType::NUMBER, "1", 1, 1.0)
            )
        );

    Token op = Token(TokenType::PLUS, "+", 1);
    BinaryExpr test_expr = BinaryExpr(left, right, op); 

    REQUIRE(test_expr.op == Token(TokenType::PLUS, "+", 1));
}


TEST_CASE("test_combine_binary_expr", "expression")
{
}

