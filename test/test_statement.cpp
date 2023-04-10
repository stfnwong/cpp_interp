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
    
    std::shared_ptr<LiteralExpr<E, T>> left = std::make_shared<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 1.0)
    );

    // TODO: Bug - the object doesn't contain a token, so we can't detect errors in the
    // token itself. I have had the lexeme "1" here and it was not caught by the test.
    // Do I want to have a unique_ptr to a Token and do a move each time the token is 
    // moved?
    std::shared_ptr<LiteralExpr<E, T>> right = std::make_shared<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "2", 1, 2.0)
    );

    BinaryExpr<E, T> out_expr(left, right, op);

    return std::make_unique<BinaryExpr<E, T>>(std::move(out_expr));
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
    std::cout << expr_stmt.expr->to_string() << std::endl;
    std::cout << typeid(expr_stmt.expr).name() << std::endl;
    std::cout << typeid(expr_stmt.expr.get()).name() << std::endl;
    //std::cout << expr_stmt.expr->left.to_string() << std::endl;

    //LiteralExpr<E,T> exp_left(TokenType::NUMBER, "2", 1, 2.0);
    //REQUIRE(*std::dynamic_pointer_cast<LiteralExpr<E, T>>(expr_stmt.expr->left).get() == exp_left);
}
