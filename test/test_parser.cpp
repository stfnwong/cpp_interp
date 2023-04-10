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
    using T = std::string;
    using E = LoxObject;

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
    //std::unique_ptr<Expr<E, T>> out_expr = parser.parse();

    //ASTPrinter printer;

    //std::string exp_parsed_str = "(+ 2 2)";
    //REQUIRE(printer.print(*out_expr) == exp_parsed_str);
}
