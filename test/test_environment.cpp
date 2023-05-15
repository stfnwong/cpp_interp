/*
 * ENVIRONMENT
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"


#include "Environment.hpp"
#include "Util.hpp"


TEST_CASE("test_create_environment", "environment")
{
    Environment env;

    // Since no vars are defined basically everything should fail
    REQUIRE_THROWS(env.get(Token(TokenType::STRING, "anything")));
    REQUIRE_THROWS(env.assign(Token(TokenType::STRING, "anything"), LoxObject(1.0f)));
}

TEST_CASE("test_create_env_with_enclosing", "environment")
{
    LoxObject outer_var_1(Token(TokenType::STRING, "1", 1));
    LoxObject outer_var_2(Token(TokenType::STRING, "2", 1));
    Token outer_var_1_name(TokenType::IDENTIFIER, "outer_var_1");
    Token outer_var_2_name(TokenType::IDENTIFIER, "outer_var_2");

    LoxObject inner_var_1(Token(TokenType::STRING, "10", 1));
    LoxObject inner_var_2(Token(TokenType::STRING, "20", 1));
    Token inner_var_1_name(TokenType::IDENTIFIER, "inner_var_1");
    Token inner_var_2_name(TokenType::IDENTIFIER, "inner_var_2");

    Environment outer;
    outer.define(outer_var_1_name.lexeme, outer_var_1);
    outer.define(outer_var_2_name.lexeme, outer_var_2);
    REQUIRE(outer.get(outer_var_1_name) == outer_var_1);

    // Create inner Environment
    Environment inner = Environment(std::make_shared<Environment>(outer));

    // Add new vars to inner scope
    inner.define(inner_var_1_name.lexeme, inner_var_1);
    inner.define(inner_var_2_name.lexeme, inner_var_2);

    REQUIRE(inner.get(inner_var_1_name) == inner_var_1);
    REQUIRE(inner.get(inner_var_2_name) == inner_var_2);

    // We can still access the variables in the outer scope
    REQUIRE(inner.get(outer_var_1_name) == outer_var_1);
    REQUIRE(inner.get(outer_var_2_name) == outer_var_2);

    // If we shadow an outer var with an inner var, we should get the inner var
    LoxObject outer_var_shadow_val(Token(TokenType::STRING, "inner shadow", 1));
    
    inner.assign(outer_var_1_name, outer_var_shadow_val);
    REQUIRE(inner.get(outer_var_1_name) == outer_var_shadow_val);
}

// Test shadowing?
TEST_CASE("test_empty_outer", "environment")
{
    // Test that we can get variables in inner scope when outer scope is empty.
    Environment outer;
    Environment inner(std::make_shared<Environment>(outer));

    // add some vars 
    LoxObject inner_1(Token(TokenType::STRING, "1", 1));
    LoxObject inner_2(Token(TokenType::STRING, "2", 1));
    Token inner_name_1(TokenType::IDENTIFIER, "outer_var_1");
    Token inner_name_2(TokenType::IDENTIFIER, "outer_var_2");

    inner.define(inner_name_1.lexeme, inner_1);
    inner.define(inner_name_2.lexeme, inner_2);
}


TEST_CASE("test_empty_inner", "environment")
{
    // Test that we can get variables in outer scope when inner scope is empty.
    //Environment* outer = new Environment();
    std::shared_ptr<Environment> outer = std::make_shared<Environment>();

    // add some vars 
    LoxObject outer_1(Token(TokenType::STRING, "1", 1));
    LoxObject outer_2(Token(TokenType::STRING, "2", 1));
    Token outer_name_1(TokenType::IDENTIFIER, "outer_var_1");
    Token outer_name_2(TokenType::IDENTIFIER, "outer_var_2");

    outer->define(outer_name_1.lexeme, outer_1);
    outer->define(outer_name_2.lexeme, outer_2);

    Environment inner(std::make_shared<Environment>(outer));

    REQUIRE(inner.get(outer_name_1) == outer_1);
    REQUIRE(inner.get(outer_name_2) == outer_2);
}



TEST_CASE("test_add_variable", "environment")
{
    Environment env;
    LoxObject var(Token(TokenType::NUMBER, "10", 1, 10.0f));

    env.define("some_var", var);

    // Get an object that exists
    LoxObject out_obj = env.get(Token(TokenType::IDENTIFIER, "some_var"));

    REQUIRE(out_obj.has_value() == true);
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
    REQUIRE(out_obj.has_value() == true);
    REQUIRE(out_obj.has_number_type() == true);
    REQUIRE(double_equal(out_obj.get_double_val(), 4.0f));

    env.assign(name, new_value);
    out_obj = env.get(name);
    REQUIRE(out_obj.has_value() == true);
    REQUIRE(out_obj.has_number_type() == true);
    REQUIRE(double_equal(out_obj.get_double_val(), 444.0f));
}

TEST_CASE("test_copy_assign", "environment")
{
    Environment base;

    // add some vars 
    LoxObject base_1(Token(TokenType::STRING, "1", 1));
    LoxObject base_2(Token(TokenType::STRING, "2", 1));
    Token base_name_1(TokenType::IDENTIFIER, "base_var_1");
    Token base_name_2(TokenType::IDENTIFIER, "base_var_2");

    base.define(base_name_1, base_1);

    Environment base_copy = base;
    REQUIRE(base_copy.get(base_name_1) == base_1);

    base.define(base_name_2, base_2);
    REQUIRE(base.get(base_name_1) == base_1);
    REQUIRE(base.get(base_name_2) == base_2);

    REQUIRE_THROWS(base_copy.get(base_name_2));

    // Now copy back
    base = base_copy;
    REQUIRE_THROWS(base.get(base_name_2));
    REQUIRE(base.get(base_name_1) == base_1);
}


TEST_CASE("test_copy_assignment_nested", "environment")
{
    Environment base;

    // add some vars 
    LoxObject base_1(Token(TokenType::STRING, "1", 1));
    LoxObject base_2(Token(TokenType::STRING, "2", 1));
    Token base_name_1(TokenType::IDENTIFIER, "base_var_1");
    Token base_name_2(TokenType::IDENTIFIER, "base_var_2");

    base.define(base_name_1, base_1);

    Environment inner = Environment(std::make_shared<Environment>(base));

    LoxObject inner_1(Token(TokenType::STRING, "1", 1));
    Token inner_name_1(TokenType::IDENTIFIER, "inner_var_1");

    inner.define(inner_name_1, inner_1);
    REQUIRE(inner.get(inner_name_1) == inner_1);

    REQUIRE_THROWS(base.get(inner_name_1));

    // Now copy inner env
    Environment inner_copy = inner;
    REQUIRE(inner_copy.get(inner_name_1) == inner_1);

    LoxObject inner_copy_1(Token(TokenType::STRING, "1", 1));
    Token inner_copy_name_1(TokenType::IDENTIFIER, "inner_copy_var_1");

    inner_copy.define(inner_copy_name_1, inner_copy_1);
    REQUIRE(inner_copy.get(inner_copy_name_1) == inner_copy_1);
    REQUIRE_THROWS(inner.get(inner_copy_name_1));
    REQUIRE_THROWS(base.get(inner_copy_name_1));
}
