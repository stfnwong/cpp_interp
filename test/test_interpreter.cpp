/*
 * INTERPRETER
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include "Interpreter.hpp"


TEST_CASE("test_interpret_binary_expr", "interpreter")
{
    // Expression is 1 + 1 
    Token op(TokenType::PLUS, "+");
    
    std::unique_ptr<LiteralExpr<E, T>> left = std::make_unique<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 1.0)
    );

    std::unique_ptr<LiteralExpr<E, T>> right = std::make_unique<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 1.0)
    );

    auto expr_ptr = std::make_unique<BinaryExpr<E, T>>(std::move(left), std::move(right), op);
    //BinaryExpr<E, T> test_expr(std::move(left), std::move(right), op);
    //REQUIRE(test_expr.op == Token(TokenType::PLUS, "+"));
    

    Interpreter interp;

    //auto exp_ptr = std::make_unique<BinaryExpr<E, T>>(test_expr);
    std::string out_eval = interp.interpret(std::move(expr_ptr));
    //std::string out_eval = interp.interpret(std::make_unique<BinaryExpr<E, T>>(test_expr));
    std::cout << "[" << __func__ << "] out_eval : " << out_eval << std::endl;

    REQUIRE(out_eval == "2");
}
