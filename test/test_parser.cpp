/*
 * PARSER
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream>
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
    // expression is "2 + 2"
    std::vector<Token> test_tokens = {
        Token(TokenType::NUMBER, "2", 1, 2.0f),
        Token(TokenType::PLUS, "+", 1),
        Token(TokenType::NUMBER, "2", 1, 2.0f),
        Token(TokenType::SEMICOLON, ";", 1),
        Token(TokenType::LOX_EOF, "", 1),
    };
    Parser parser(test_tokens);
    REQUIRE(parser.num_tokens() == test_tokens.size());
    std::string exp_string_repr = "ExpressionStmt<2 + 2>";

    // Should return an expression
    auto parse_result = parser.parse();
    REQUIRE(parse_result.size() == 1);
    REQUIRE(parse_result[0]->to_string() == exp_string_repr);
}
