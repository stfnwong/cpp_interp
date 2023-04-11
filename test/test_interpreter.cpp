/*
 * INTERPRETER
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include "Interpreter.hpp"
#include "Parser.hpp"


//TEST_CASE("test_interpret_binary_expr", "interpreter")
//{
//    // Expression is 1 + 1 
//    Token op(TokenType::PLUS, "+");
//    
//    std::unique_ptr<LiteralExpr<E, T>> left = std::make_unique<LiteralExpr<E, T>>(
//            Token(TokenType::NUMBER, "1", 1, 1.0)
//    );
//
//    std::unique_ptr<LiteralExpr<E, T>> right = std::make_unique<LiteralExpr<E, T>>(
//            Token(TokenType::NUMBER, "1", 1, 1.0)
//    );
//
//    auto expr_ptr = std::make_unique<BinaryExpr<E, T>>(std::move(left), std::move(right), op);
//    
//    Interpreter interp;
//    std::string out_eval = interp.interpret(std::move(expr_ptr));
//    std::string exp_eval_str = "2";
//
//    REQUIRE(out_eval == exp_eval_str);
//}


TEST_CASE("test_parse_and_interpret", "interpreter")
{
    std::vector<Token> test_tokens = {
        Token(TokenType::NUMBER, "2", 1, 2.0f),
        Token(TokenType::PLUS, "+", 1),
        Token(TokenType::NUMBER, "2", 1, 2.0f),
        Token(TokenType::SEMICOLON, ";", 1),
        Token(TokenType::LOX_EOF, "", 1),
    };
    Parser parser(test_tokens);
    REQUIRE(parser.num_tokens() == test_tokens.size());

    auto parsed_output = parser.parse();

    Interpreter interp;

    interp.interpret(parsed_output);

}
