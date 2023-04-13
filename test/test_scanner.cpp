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

    std::vector<Token> tokens = test_scanner.scan();

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
    std::vector<Token> tokens = test_scanner.scan();

    REQUIRE(tokens.size() == exp_tokens.size());
    for(unsigned i = 0; i < tokens.size(); ++i)
        REQUIRE(tokens[i] == exp_tokens[i]);
}

TEST_CASE("test_scan_number_literal", "scanner")
{
    std::string test_source = "1.0, 0.0, 20, 40";
    std::vector<Token> exp_tokens = {
        Token(TokenType::NUMBER, "1.0", 1, 1.0f),
        Token(TokenType::COMMA, ",", 1),
        Token(TokenType::NUMBER, "0.0", 1, 0.0f),
        Token(TokenType::COMMA, ",", 1),
        Token(TokenType::NUMBER, "20", 1, 20.0f),
        Token(TokenType::COMMA, ",", 1),
        Token(TokenType::NUMBER, "40", 1, 40.0f),
        Token(TokenType::LOX_EOF, "", 1),
    };

    Scanner test_scanner(test_source);
    std::vector<Token> tokens = test_scanner.scan();

    REQUIRE(tokens.size() == exp_tokens.size());
    for(unsigned i = 0; i < tokens.size(); ++i)
        REQUIRE(tokens[i] == exp_tokens[i]);

}

TEST_CASE("test_scan_string_literal", "scanner")
{
    std::string test_source = "\"some string literal\"";
    std::vector<Token> exp_tokens = {
        Token(TokenType::STRING, "some string literal", 1),
        Token(TokenType::LOX_EOF, "", 1),
    };

    Scanner test_scanner(test_source);
    std::vector<Token> tokens = test_scanner.scan();

    REQUIRE(tokens.size() == exp_tokens.size());
    for(unsigned i = 0; i < tokens.size(); ++i)
        REQUIRE(tokens[i] == exp_tokens[i]);
}

TEST_CASE("test_scan_keyword", "scanner")
{
    std::string test_source = "and class else false fun for if nil or print return super this true var while";

    std::vector<Token> exp_tokens = {
        Token(TokenType::AND, "and", 1),
        Token(TokenType::CLASS, "class", 1),
        Token(TokenType::ELSE, "else", 1),
        Token(TokenType::FALSE, "false", 1),
        Token(TokenType::FUN, "fun", 1),
        Token(TokenType::FOR, "for", 1),
        Token(TokenType::IF, "if", 1),
        Token(TokenType::NIL, "nil", 1),
        Token(TokenType::OR, "or", 1),
        Token(TokenType::PRINT, "print", 1),
        Token(TokenType::RETURN, "return", 1),
        Token(TokenType::SUPER, "super", 1),
        Token(TokenType::THIS, "this", 1),
        Token(TokenType::TRUE, "true", 1),
        Token(TokenType::VAR, "var", 1),
        Token(TokenType::WHILE, "while", 1),
        Token(TokenType::LOX_EOF, "", 1),
    };

    Scanner test_scanner(test_source);
    std::vector<Token> tokens = test_scanner.scan();

    REQUIRE(tokens.size() == exp_tokens.size());
    for(unsigned i = 0; i < tokens.size(); ++i)
        REQUIRE(tokens[i] == exp_tokens[i]);
}

TEST_CASE("test_scan_identifier", "scanner")
{
    std::string test_source = "var some_identifier; var another_identifier;";
    std::vector<Token> exp_tokens = {
        Token(TokenType::VAR, "var", 1),
        Token(TokenType::IDENTIFIER, "some_identifier", 1),
        Token(TokenType::SEMICOLON, ";", 1),
        Token(TokenType::VAR, "var", 1),
        Token(TokenType::IDENTIFIER, "another_identifier", 1),
        Token(TokenType::SEMICOLON, ";", 1),
        Token(TokenType::LOX_EOF, "", 1),
    };

    Scanner test_scanner(test_source);
    std::vector<Token> tokens = test_scanner.scan();

    REQUIRE(tokens.size() == exp_tokens.size());
    for(unsigned i = 0; i < tokens.size(); ++i)
        REQUIRE(tokens[i] == exp_tokens[i]);
}
