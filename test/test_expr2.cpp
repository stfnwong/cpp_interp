/*
 * EXPRESSIONS 2: ELECTRIC BOOGALOO
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream>

#include "Expr2.hpp"
#include "Token.hpp"
#include "Util.hpp"


using T = std::string;


TEST_CASE("test_create_literal_expr", "expr2")
{
    T test_string = "literal2: electric boogaloo";
    Token test_token(TokenType::STRING, test_string, 1, test_string);
    LiteralExpr2<LoxObject> test_expr(test_token);

    REQUIRE(test_expr.value.get_string_val() == test_string);
}


TEST_CASE("test_create_unary_expr", "expr2")
{
    Token op(TokenType::MINUS, "-");
    std::shared_ptr<LiteralExpr2<T>> left = std::make_shared<LiteralExpr2<T>>(
            Token(TokenType::NUMBER, "1", 1, 1.0)
    );

    UnaryExpr2<T> test_expr(left, op);

    REQUIRE(test_expr.op == Token(TokenType::MINUS, "-"));
}


TEST_CASE("test_create_binary_expr", "expr2")
{
    Token op(TokenType::PLUS, "+");
    
    std::shared_ptr<LiteralExpr2<T>> left = std::make_shared<LiteralExpr2<T>>(
            Token(TokenType::NUMBER, "1", 1, 1.0)
    );

    std::shared_ptr<LiteralExpr2<T>> right = std::make_shared<LiteralExpr2<T>>(
            Token(TokenType::NUMBER, "1", 1, 2.0)
    );

    BinaryExpr2<T> test_expr(left, right, op);
    
    REQUIRE(test_expr.op == Token(TokenType::PLUS, "+"));

    LiteralExpr2<T> exp_left(Token(TokenType::NUMBER, "1", 1, 1.0));
    LiteralExpr2<T> exp_right(Token(TokenType::NUMBER, "2", 1, 2.0));

    REQUIRE(*std::dynamic_pointer_cast<LiteralExpr2<T>>(test_expr.left).get() == exp_left);
    REQUIRE(*std::dynamic_pointer_cast<LiteralExpr2<T>>(test_expr.right).get() == exp_right);
}


