/*
 * PARSER
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <vector>

#include "Parser.hpp"


TEST_CASE("test_parse_empty_input", "parser")
{
    std::vector<Token> empty_input;
    Parser parser(empty_input);

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("test_parse_binary_expr", "parser")
{
    // expression is 
    std::vector<Token> test_tokens = {
        Token(TokenType::NUMBER, "2", 1, 2.0f),
        Token(TokenType::PLUS, "+", 1),
        Token(TokenType::NUMBER, "2", 1, 2.0f),
        Token(TokenType::LOX_EOF, "", 1),
    };
    Parser parser(test_tokens);

    REQUIRE(parser.num_tokens() == test_tokens.size());

    // Should return an expression
    //std::unique_ptr<BinaryExpr> out_expr = dynamic_cast<BinaryExpr>(parser.parse().get());
    std::unique_ptr<Expr> out_expr = parser.parse();
    std::cout << "[" << __func__ << "] out_expr (type: " << typeid(out_expr.get()).name()
        << "): " << out_expr->to_string() << std::endl;

    // We expect the binary expression (2 + 2)
    //REQUIRE(out_expr->op == Token(TokenType::PLUS, "+", 1, 2.0f));

    //LiteralExpr exp_left = LiteralExpr(
    //        Object(
    //            Token(TokenType::NUMBER, "2", 1, 2.0f)
    //        )
    //);

    //LiteralExpr exp_right = LiteralExpr(
    //        Object(
    //            Token(TokenType::NUMBER, "2", 1, 2.0f)
    //        )
    //);

    //REQUIRE(out_expr->left.get() == exp_left);
    //REQUIRE(out_expr->right.get() == exp_right);
}
