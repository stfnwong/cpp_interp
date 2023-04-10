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
    std::unique_ptr<LiteralExpr<E, T>> left = std::make_unique<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 1.0)
    );

    UnaryExpr<E, T> test_expr(std::move(left), op);
    LiteralExpr<E, T> exp_value(Token(TokenType::NUMBER, "1", 1, 1.0));

    REQUIRE(test_expr.op == Token(TokenType::MINUS, "-"));
    //REQUIRE(*std::dynamic_pointer_cast<LiteralExpr<E, T>>(test_expr.right).get() == exp_value);
}


TEST_CASE("test_create_binary_expr", "expr")
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

    BinaryExpr<E, T> test_expr(std::move(left), std::move(right), op);
    
    REQUIRE(test_expr.op == Token(TokenType::PLUS, "+"));

    LiteralExpr<E, T> exp_left(Token(TokenType::NUMBER, "1", 1, 1.0));
    LiteralExpr<E, T> exp_right(Token(TokenType::NUMBER, "2", 1, 2.0));

    // These cast can be handy in that we can use the == operator directly, but when
    // implementing components using this use the get_*() methods instead (so that 
    // its not required to know the type of the derived object).
    //REQUIRE(*std::dynamic_pointer_cast<LiteralExpr<E, T>>(test_expr.left).get() == exp_left);
    //REQUIRE(*std::dynamic_pointer_cast<LiteralExpr<E, T>>(test_expr.right).get() == exp_right);
}


TEST_CASE("test_ast_printer", "expr")
{
    ASTPrinter printer;

    // Create some expression
    Token op(TokenType::PLUS, "+");
    
    std::unique_ptr<LiteralExpr<E, T>> left = std::make_unique<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 1.0)
    );

    std::unique_ptr<LiteralExpr<E, T>> right = std::make_unique<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 2.0)  // TODO: put token in object so this doesn't pass
    );

    BinaryExpr<E, T> test_expr(std::move(left), std::move(right), op);
    
    REQUIRE(test_expr.op == Token(TokenType::PLUS, "+"));
    REQUIRE(printer.print(test_expr) == "(+ 1 2)");
}
