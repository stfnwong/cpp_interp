/*
 * CALLABLE
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include "Scanner.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"
#include "Stdlib.hpp"


TEST_CASE("test_clock", "stdlib")
{
    std::string source = "clock();";
    Scanner scanner(source);
    Parser parser(scanner.scan());
    auto statements = parser.parse();

    REQUIRE(statements.size() == 1);

    Interpreter interp;
    LoxObject res = interp.execute(statements[0]);

    REQUIRE(res.has_type() == true);
    REQUIRE(res.has_number_type() == true);
    // We don't really care about the value since its just going to be the moment 
    // when the test is run.
}
