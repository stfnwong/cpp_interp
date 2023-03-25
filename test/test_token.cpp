// Token Test

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include "Token.hpp"


TEST_CASE("test_create_token", "token")
{
    // Construct a token that holds a string literal
    Token test_token(TokenType::VAR, "some_var", 1);

    REQUIRE(test_token.type == TokenType::VAR);
    REQUIRE(test_token.lexeme == "some_var");
    REQUIRE(test_token.line == 1);
    REQUIRE(test_token.has_literal() == false);
    REQUIRE(test_token.get_string_literal() == "");
}

TEST_CASE("test_create_token_with_string_literal", "token")
{
    std::string test_literal = "heyyyy im a string literal im fucken walking hereeee";
    Token test_token(TokenType::VAR, "some_var", 1, test_literal);

    REQUIRE(test_token.has_literal() == true);
    REQUIRE(test_token.get_string_literal() == test_literal);
    REQUIRE_THROWS(test_token.get_float_literal());
}

TEST_CASE("test_create_token_with_float_literal", "token")
{
    Token test_token(TokenType::VAR, "1.0", 1, 1.0f);

    REQUIRE(test_token.has_literal() == true);
    REQUIRE(test_token.get_float_literal() == 1.0f);
    REQUIRE_THROWS(test_token.get_string_literal());
}

TEST_CASE("test_copy_token", "token")
{
    Token src_token;

    REQUIRE(src_token.type == TokenType::EMPTY);
    REQUIRE(src_token.line == 0);

    // can we copy another token by assignment?
    Token dst_token = Token(TokenType::VAR, "some_var", 1);
    REQUIRE(dst_token.type == TokenType::VAR);
    REQUIRE(dst_token.line == 1);

    src_token = dst_token;
    REQUIRE(src_token.line == 1);
    REQUIRE(src_token.type == TokenType::VAR);
}

// TODO: copy construction
