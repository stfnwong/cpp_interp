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
    REQUIRE(test_object.src_type == TokenType::EMPTY);
}


//TEST_CASE("test_create_bool_object", "object")
//{
//    Token test_token = Token(TokenType::TRUE, "true", 1);
//    LoxObject test_object(test_token);
//
//    REQUIRE(test_object.has_type() == true);
//    REQUIRE(test_object.src_type == test_token.type);
//
//    REQUIRE_THROWS(test_object.get_string_val());
//    REQUIRE_THROWS(test_object.get_float_val());
//
//    REQUIRE(test_object.get_bool_val() == test_token.get_bool_literal());
//    // False should be fine also, but when getting token literals or 
//    // object values we return false when we fail (eg: when the std::optional is empty)
//    test_token = Token(TokenType::FALSE, "false", 1);
//    test_object = LoxObject(test_token);
//    REQUIRE(test_object.has_type() == true);
//    REQUIRE(test_object.src_type == test_token.type);
//    REQUIRE(test_object.get_bool_val() == test_token.get_bool_literal());
//}

TEST_CASE("test_create_number_object", "object")
{
    Token test_token = Token(TokenType::NUMBER, "5", 5.0);
    LoxObject test_object(test_token);

    REQUIRE(test_object.has_type() == true);
    REQUIRE(test_object.src_type == test_token.type);

    REQUIRE_THROWS(test_object.get_string_val());
    REQUIRE_THROWS(test_object.get_bool_val());

    REQUIRE(float_equal(test_object.get_float_val(), test_token.get_float_literal()));
}
