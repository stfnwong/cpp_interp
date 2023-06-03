/*
 * RESOLVER
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream>

#include "Scanner.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"
#include "Resolver.hpp"



std::vector<std::unique_ptr<Stmt<EType, VType>>> get_token_stream(const std::string& source)
{
    Scanner scanner(source);
    auto scanned_tokens = scanner.scan();
    Parser parser(scanned_tokens);

    return parser.parse();
}



TEST_CASE("test_resolve_nested_scope", "resolver")
{
    Interpreter interp;
    Resolver<EType, VType> resolver(interp);


}
