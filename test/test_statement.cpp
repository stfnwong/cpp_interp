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
    // Take apart the expressions and check
    std::cout << "[" << __func__ << "] expr_stmt.expr : " << typeid(expr_stmt.expr).name() << std::endl;
    std::cout << "[" << __func__ << "] expr_stmt.expr : " << typeid(expr_stmt.expr.get()).name() << std::endl;
    std::cout << "[" << __func__ << "] expr_stmt.expr : " << expr_stmt.expr->to_string() << std::endl;
    //std::cout << typeid(static_cast<LiteralExpr<E, T>>(expr_stmt.expr.get())).name() << std::endl;
    //REQUIRE(*std::dynamic_pointer_cast<LiteralExpr<E, T>>(expr_stmt.expr.get()).get()->right->value == LoxObject(1.0f));


    // I am downcasting the pointers by hand here to make it possible to write a single
    // REQUIRES() macro that uses the == operator. Its not suggested that this is good
    // practice in general.
    BinaryExpr<E, T>* derived_ptr = static_cast<BinaryExpr<E, T>*>(expr_stmt.expr.get());
    std::cout << "[" << typeid(derived_ptr).name() << "]: " << derived_ptr->to_string() << std::endl;

    std::cout << derived_ptr->left->to_string() << std::endl;
    std::cout << derived_ptr->right->to_string() << std::endl;
    std::cout << derived_ptr->op.to_string() << std::endl;

    REQUIRE(static_cast<LiteralExpr<E, T>*>(derived_ptr->left.get())->value == LoxObject(1.0f));
    REQUIRE(static_cast<LiteralExpr<E, T>*>(derived_ptr->right.get())->value == LoxObject(2.0f));
    REQUIRE(derived_ptr->op == Token(TokenType::PLUS, "+"));

    //std::cout << typeid(static_cast<BinaryExpr<E, T>>(expr_stmt.expr.get())).name() 
    //    //<< "(" << typeid(static_cast<BinaryExpr<E, T>>(expr_stmt.expr.get()).name() << ")"
    //    << ": [" << static_cast<BinaryExpr<E, T>>(expr_stmt.expr.get()).to_string() << "]"
    //    << static_cast<BinaryExpr<E, T>>(expr_stmt.expr.get()).value.to_string() << std::endl;

    //REQUIRE(static_cast<BinaryExpr<E, T>>(expr_stmt.expr.get()).value == LoxObject(1.0f));


    //REQUIRE(expr->get_left()->get_value() == LoxObject(1.0f));
    //REQUIRE(expr->get_right()->get_value() == LoxObject(2.0f));
    //REQUIRE(expr->get_op() == Token(TokenType::PLUS, "+"));
}
