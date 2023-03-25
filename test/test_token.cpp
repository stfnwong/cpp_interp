// Token Test

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include "Token.hpp"


TEST_CASE("test_create_token", "token")
{
    // Construct a token that holds a string literal
    Token test_token_string(TokenType::VAR, "some_var", 1);

    REQUIRE(test_token_string.type == TokenType::VAR);
    REQUIRE(test_token_string.lexeme == "some_var");
    REQUIRE(test_token_string.line == 1);
    REQUIRE(test_token_string.literal.has_value() == false);
}

TEST_CASE("test_create_token_with_string_literal", "token")
{
    std::string test_literal = "heyyyy im a string literal im fucken walking hereeee";
    Token test_token_string(TokenType::VAR, "some_var", 1, test_literal);

    REQUIRE(test_token_string.has_literal() == true);
    //REQUIRE(test_token_string.literal.value() == test_literal);
}
