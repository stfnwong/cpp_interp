/*
 * EXPRESSIONS
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"


#include "Expr.hpp"
#include "Token.hpp"
#include "Util.hpp"


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

    //REQUIRE(test_expr.left.get()->type == TokenType::NUMBER);
    //REQUIRE(float_equal(test_expr.left->get_float_value(), 1.0));
}


TEST_CASE("test_combine_binary_expr", "expression")
{
    // Concatenate two binary expressions together
}


TEST_CASE("test_print_ast", "expression")
{
    ASTPrinter printer;

    // Try to print the expression "1 + 2". We expect the output to be in RPN format
    // (+ 1 2)

    std::unique_ptr<LiteralExpr> left = std::make_unique<LiteralExpr>(
            LoxObject(
                Token(TokenType::NUMBER, "1", 1, 1.0)
            )
    );

    std::unique_ptr<LiteralExpr> right = std::make_unique<LiteralExpr>(
            LoxObject(
                Token(TokenType::NUMBER, "2", 1, 2.0)
            )
    );

    Token op(TokenType::PLUS, "+", 1);
    BinaryExpr test_expr(std::move(left), std::move(right), op);

    std::cout << "[" << __func__ << "] test expression: " << test_expr.to_string() << std::endl;

    std::string expr_str = printer.print(test_expr);
    std::cout << "[" << __func__ << "] expr_str: " << expr_str << std::endl;
}
