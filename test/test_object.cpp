/*
 * OBJECT
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <sstream>

#include "Util.hpp"
#include "Object.hpp"



TEST_CASE("test_create_null_object", "object")
{
    LoxObject test_object;

    REQUIRE(test_object.has_value() == false);
    REQUIRE(test_object.type == ObjType::NIL);
    REQUIRE(test_object.token.type == TokenType::EMPTY);

    REQUIRE_THROWS(test_object.get_string_val());
    REQUIRE_THROWS(test_object.get_bool_val());
    REQUIRE_THROWS(test_object.get_double_val());
}


TEST_CASE("test_create_number_object", "object")
{
    double val = 5.0f;
    Token test_token = Token(TokenType::NUMBER, "5", val);
    LoxObject test_object(test_token);

    REQUIRE(test_object.has_value() == true);
    REQUIRE(test_object.type == ObjType::NUMBER);

    REQUIRE_THROWS(test_object.get_string_val());
    REQUIRE_THROWS(test_object.get_bool_val());

    REQUIRE(double_equal(test_object.get_double_val(), test_token.get_double_literal()));
}


TEST_CASE("test_create_string_object", "object")
{
    std::string test_string = "some string";
    Token test_token = Token(TokenType::STRING, test_string, 1, test_string);
    LoxObject test_object(test_token);

    REQUIRE(test_object.has_value() == true);
    REQUIRE(test_object.type == ObjType::STRING);
    //REQUIRE(test_object.get_type() == test_token.type);

    REQUIRE_THROWS(test_object.get_double_val());
    REQUIRE_THROWS(test_object.get_bool_val());

    REQUIRE(test_object.get_string_val() == test_string);
}

TEST_CASE("test_object_equality_operator", "object")
{
    Token tok_a = Token(TokenType::NUMBER, "5", 1, 5.0f);
    Token tok_b = Token(TokenType::NUMBER, "6", 1, 6.0f);

    LoxObject obj_a(tok_a);
    LoxObject obj_b(tok_b);

    REQUIRE(obj_a != obj_b);
}


// It would be nice to be able to use these objects as generic containers for the
// types we encounter when parsing. 
TEST_CASE("test_object_create_double", "object")
{
    LoxObject test_object(1.0f);

    REQUIRE(test_object.has_value() == true);
    REQUIRE(test_object.type == ObjType::NUMBER);
    REQUIRE(double_equal(test_object.get_double_val(), 1.0f));

    REQUIRE_THROWS(test_object.get_string_val());
    REQUIRE_THROWS(test_object.get_bool_val());
}

TEST_CASE("test_object_create_string", "object")
{
    std::string test_string = "some junk";
    LoxObject test_object(test_string);

    REQUIRE(test_object.has_value() == true);
    REQUIRE(test_object.type == ObjType::STRING);
    //REQUIRE(test_object.get_type() == TokenType::STRING);
    REQUIRE(test_object.get_string_val() == test_string);

    REQUIRE_THROWS(test_object.get_double_val());
    REQUIRE_THROWS(test_object.get_bool_val());
}


TEST_CASE("test_object_create_bool", "object")
{
    double a = 1.0f;
    double b = 2.0f;
    LoxObject true_object(a < b);

    REQUIRE(true_object.has_value() == true);
    REQUIRE(true_object.type == ObjType::BOOLEAN);
    REQUIRE(true_object.token.type == TokenType::TRUE);
    REQUIRE(true_object.get_bool_val() == true);

    REQUIRE_THROWS(true_object.get_double_val());
    REQUIRE_THROWS(true_object.get_string_val());

    LoxObject false_object(a > b);
    REQUIRE(false_object.has_value() == true);
    REQUIRE(false_object.type == ObjType::BOOLEAN);
    REQUIRE(false_object.token.type == TokenType::FALSE);
    REQUIRE(false_object.get_bool_val() == false);

    REQUIRE_THROWS(false_object.get_double_val());
    REQUIRE_THROWS(false_object.get_string_val());
}


//TEST_CASE("test_create_callable_object", "object")
//{
//}
