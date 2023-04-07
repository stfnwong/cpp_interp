/*
 * EXPRESSIONS 2: ELECTRIC BOOGALOO
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream>

#include "Expr.hpp"
#include "Token.hpp"
#include "Util.hpp"


using T = std::string;
using E = LoxObject;


TEST_CASE("test_create_literal_expr", "expr")
{
    T test_string = "literal2: electric boogaloo";
    Token test_token(TokenType::STRING, test_string, 1, test_string);
    LiteralExpr<E, T> test_expr(test_token);

    REQUIRE(test_expr.value.get_string_val() == test_string);
}


TEST_CASE("test_create_unary_expr", "expr")
{
    Token op(TokenType::MINUS, "-");
    std::shared_ptr<LiteralExpr<E, T>> left = std::make_shared<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 1.0)
    );

    UnaryExpr<E, T> test_expr(left, op);

    REQUIRE(test_expr.op == Token(TokenType::MINUS, "-"));

    LiteralExpr<E, T> exp_value(Token(TokenType::NUMBER, "1", 1, 1.0));

    REQUIRE(*std::dynamic_pointer_cast<LiteralExpr<E, T>>(test_expr.value).get() == exp_value);
}


TEST_CASE("test_create_binary_expr", "expr")
{
    Token op(TokenType::PLUS, "+");
    
    std::shared_ptr<LiteralExpr<E, T>> left = std::make_shared<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 1.0)
    );

    std::shared_ptr<LiteralExpr<E, T>> right = std::make_shared<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 2.0)
    );

    BinaryExpr<E, T> test_expr(left, right, op);
    
    REQUIRE(test_expr.op == Token(TokenType::PLUS, "+"));

    LiteralExpr<E, T> exp_left(Token(TokenType::NUMBER, "1", 1, 1.0));
    LiteralExpr<E, T> exp_right(Token(TokenType::NUMBER, "2", 1, 2.0));

    REQUIRE(*std::dynamic_pointer_cast<LiteralExpr<E, T>>(test_expr.left).get() == exp_left);
    REQUIRE(*std::dynamic_pointer_cast<LiteralExpr<E, T>>(test_expr.right).get() == exp_right);
}


//TEST_CASE("test_create_grouping_expr", "expr")
//{
//    std::shared_ptr<LiteralExpr<E, T>> number = std::make_shared<LiteralExpr<T>>(
//            Token(TokenType::NUMBER, "1", 1, 1.0)
//    );
//}


TEST_CASE("test_ast_printer", "expr")
{
    ASTPrinter printer;

    // Create some expression
    Token op(TokenType::PLUS, "+");
    
    std::shared_ptr<LiteralExpr<E, T>> left = std::make_shared<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 1.0)
    );

    std::shared_ptr<LiteralExpr<E, T>> right = std::make_shared<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 2.0)
    );

    BinaryExpr<E, T> test_expr(left, right, op);
    
    REQUIRE(test_expr.op == Token(TokenType::PLUS, "+"));
    std::string expr_out = printer.print(test_expr);
    
    REQUIRE(expr_out == "(+ 1 2)");
}
