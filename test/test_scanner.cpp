/*
 * SCANNER
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include "Scanner.hpp"


TEST_CASE("test_create_scanner", "scanner")
{
    std::string test_source = "{( )}";
    Scanner test_scanner(test_source);

    std::vector<Token> tokens = test_scanner.scan_tokens();
    std::vector<Token> exp_tokens = {
        Token(TokenType::LEFT_BRACE, "{", 1),
        Token(TokenType::LEFT_PAREN, "(", 1),
        Token(TokenType::RIGHT_PAREN, ")", 1),
        Token(TokenType::RIGHT_BRACE, "}", 1),
        Token(TokenType::LOX_EOF, "", 1),
    };
    for(unsigned i = 0; i < tokens.size(); ++i)
        std::cout << "Got: " << tokens[i].to_string() << std::endl;

    REQUIRE(tokens.size() == exp_tokens.size());
    for(unsigned i = 0; i < tokens.size(); ++i)
    {
        std::cout << "Got: " << tokens[i].to_string() << ", Exp: " << exp_tokens[i].to_string() << std::endl;
        REQUIRE(tokens[i] == exp_tokens[i]);
    }
}
