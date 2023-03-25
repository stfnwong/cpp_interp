/*
 * SCANNER
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include "Scanner.hpp"


TEST_CASE("test_scan_one_character_tokens", "scanner")
{
    std::string test_source = "{( )} , . - + ; * < > ! = /";
    // TODO: add all single character tokens 
    std::vector<Token> exp_tokens = {
        Token(TokenType::LEFT_BRACE, "{", 1),
        Token(TokenType::LEFT_PAREN, "(", 1),
        Token(TokenType::RIGHT_PAREN, ")", 1),
        Token(TokenType::RIGHT_BRACE, "}", 1),
        Token(TokenType::COMMA, ",", 1),
        Token(TokenType::DOT, ".", 1),
        Token(TokenType::MINUS, "-", 1),
        Token(TokenType::PLUS, "+", 1),
        Token(TokenType::SEMICOLON, ";", 1),
        Token(TokenType::STAR, "*", 1),
        Token(TokenType::LESS, "<", 1),
        Token(TokenType::GREATER, ">", 1),
        Token(TokenType::BANG, "!", 1),
        Token(TokenType::EQUAL, "=", 1),
        Token(TokenType::SLASH, "/", 1),
        Token(TokenType::LOX_EOF, "", 1),
    };
    Scanner test_scanner(test_source);

    std::vector<Token> tokens = test_scanner.scan_tokens();

    REQUIRE(tokens.size() == exp_tokens.size());
    for(unsigned i = 0; i < tokens.size(); ++i)
        REQUIRE(tokens[i] == exp_tokens[i]);
}

TEST_CASE("test_scan_two_character_tokens", "scanner")
{
    std::string test_source = "!= == <= >= // ignore all this";

    std::vector<Token> exp_tokens = {
        Token(TokenType::BANG_EQUAL, "!=", 1),
        Token(TokenType::EQUAL_EQUAL, "==", 1),
        Token(TokenType::LESS_EQUAL, "<=", 1),
        Token(TokenType::GREATER_EQUAL, ">=", 1),
        Token(TokenType::LOX_EOF, "", 1),
    };

    Scanner test_scanner(test_source);
    std::vector<Token> tokens = test_scanner.scan_tokens();

    REQUIRE(tokens.size() == exp_tokens.size());
    for(unsigned i = 0; i < tokens.size(); ++i)
        REQUIRE(tokens[i] == exp_tokens[i]);
}
