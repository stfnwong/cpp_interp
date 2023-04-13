/*
 * ENVIRONMENT
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"


#include "Environment.hpp"

//TEST_CASE("test_create_environment", "environment")
//{
//}

TEST_CASE("test_add_variable", "environment")
{
    Environment env;
    LoxObject var(Token(TokenType::NUMBER, "10", 1, 10.0f));

    env.define("some_var", var);

    // Get an object that exists
    LoxObject out_obj = env.get(Token(TokenType::IDENTIFIER, "some_var"));

    REQUIRE(out_obj.has_type() == true);

    // Try to get an undefined variable 
    REQUIRE_THROWS(env.get(Token(TokenType::IDENTIFIER, "no_such_var")));
}

