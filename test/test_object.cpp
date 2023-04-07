/*
 * OBJECT
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include "Util.hpp"
#include "Object.hpp"



TEST_CASE("test_create_null_object", "object")
{
    LoxObject test_object;

    REQUIRE(test_object.has_type() == false);
    REQUIRE(test_object.type == TokenType::EMPTY);

    //REQUIRE_THROWS(test_object.get_string_val());
    //REQUIRE_THROWS(test_object.get_bool_val());
    //REQUIRE_THROWS(test_object.get_float_val());
}


TEST_CASE("test_create_number_object", "object")
{
    Token test_token = Token(TokenType::NUMBER, "5", 5.0);
    LoxObject test_object(test_token);

    REQUIRE(test_object.has_type() == true);
    REQUIRE(test_object.type == test_token.type);

    REQUIRE_THROWS(test_object.get_string_val());
    REQUIRE_THROWS(test_object.get_bool_val());

    REQUIRE(float_equal(test_object.get_float_val(), test_token.get_float_literal()));
}


TEST_CASE("test_create_string_object", "object")
{
    std::string test_string = "some string";
    // TODO: what can we do about the double argument?
    Token test_token = Token(TokenType::STRING, test_string, 1, test_string);
    LoxObject test_object(test_token);

    REQUIRE(test_object.has_type() == true);
    REQUIRE(test_object.type == test_token.type);

    REQUIRE_THROWS(test_object.get_float_val());
    REQUIRE_THROWS(test_object.get_bool_val());

    REQUIRE(test_object.get_string_val() == test_string);
}

TEST_CASE("test_object_equality_operator", "object")
{
    Token tok_a = Token(TokenType::NUMBER, "5", 1, 5.0);
    Token tok_b = Token(TokenType::NUMBER, "6", 1, 6.0);

    LoxObject obj_a(tok_a);
    LoxObject obj_b(tok_b);

    REQUIRE(obj_a != obj_b);
}
