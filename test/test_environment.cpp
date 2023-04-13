/*
 * ENVIRONMENT
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"


#include "Environment.hpp"
#include "Util.hpp"


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
    REQUIRE(out_obj.has_number_type() == true);
    REQUIRE(double_equal(out_obj.get_double_val(), 10.0f));

    // Try to get an undefined variable 
    REQUIRE_THROWS(env.get(Token(TokenType::IDENTIFIER, "no_such_var")));
}


TEST_CASE("test_add_assignment", "environment")
{
    Environment env;

    // make a var
    Token name(Token(TokenType::IDENTIFIER, "whatever"));
    LoxObject value(Token(TokenType::NUMBER, "4", 1, 4.0f));
    LoxObject new_value(Token(TokenType::NUMBER, "444", 1, 444.0f));

    // We shouldn't be able to assign to variable that doesn't yet exist
    REQUIRE_THROWS(env.assign(name, value));

    // Once the value is defined we can assign
    env.define(name.lexeme, value);

    LoxObject out_obj = env.get(name);
    REQUIRE(out_obj.has_type() == true);
    REQUIRE(out_obj.has_number_type() == true);
    REQUIRE(double_equal(out_obj.get_double_val(), 4.0f));

    env.assign(name, new_value);
    out_obj = env.get(name);
    REQUIRE(out_obj.has_type() == true);
    REQUIRE(out_obj.has_number_type() == true);
    REQUIRE(double_equal(out_obj.get_double_val(), 444.0f));
}
