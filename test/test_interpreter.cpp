/*
 * INTERPRETER
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include "Interpreter.hpp"


TEST_CASE("test_interpret_expr", "interpreter")
{
    // Expression is 1 + 1 
    Token op(TokenType::PLUS, "+");
    
    std::shared_ptr<LiteralExpr<E, T>> left = std::make_shared<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 1.0)
    );

    std::shared_ptr<LiteralExpr<E, T>> right = std::make_shared<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 1.0)
    );

    BinaryExpr<E, T> test_expr(left, right, op);
    
    REQUIRE(test_expr.op == Token(TokenType::PLUS, "+"));
    

    Interpreter interp;

    std::string out_eval = interp.interpret(std::make_shared<BinaryExpr<E, T>>(test_expr));
    std::cout << "[" << __func__ << "] out_eval : " << out_eval << std::endl;

    REQUIRE(out_eval == "2");

}
