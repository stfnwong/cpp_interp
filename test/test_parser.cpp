/*
 * PARSER
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <vector>

#include "Parser.hpp"
#include "Object.hpp"


TEST_CASE("test_parse_empty_input", "parser")
{
    std::vector<Token> empty_input;
    Parser parser(empty_input);

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("test_parse_binary_expr", "parser")
{
    using T = std::string;
    using E = LoxObject;

    // expression is "2 + 2"
    std::vector<Token> test_tokens = {
        Token(TokenType::NUMBER, "2", 1, 2.0f),
        Token(TokenType::PLUS, "+", 1),
        Token(TokenType::NUMBER, "2", 1, 2.0f),
        Token(TokenType::LOX_EOF, "", 1),
    };
    Parser parser(test_tokens);
    REQUIRE(parser.num_tokens() == test_tokens.size());

    // Should return an expression
    //std::list<std::unique_ptr<Stmt<E, T>>> parse_result = parser.parse();
    auto parse_result = parser.parse();
    REQUIRE(parse_result.size() == 1);
    //auto out_expr = parse_result.front();

    //REQUIRE(out_expr->get_expr()->get_left()->get_value() == LoxObject(2.0f));
    //REQUIRE(out_expr->get_expr()->get_right()->get_value() == LoxObject(2.0f));
    //REQUIRE(out_expr->get_expr()->get_op() == Token(TokenType::PLUS, "+", 1));
}
