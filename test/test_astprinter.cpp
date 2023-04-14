/*
 * EXPRESSIONS 2: ELECTRIC BOOGALOO
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream>

#include "Scanner.hpp"
#include "Parser.hpp"

#include "ASTPrinter.hpp"
#include "Util.hpp"


using T = LoxObject;
using E = LoxObject;



TEST_CASE("test_ast_printer", "ast_printer")
{
    ASTPrinter printer;

    // Create some expression
    Token op(TokenType::PLUS, "+");
    
    std::unique_ptr<LiteralExpr<E, T>> left = std::make_unique<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 1.0f)
    );

    std::unique_ptr<LiteralExpr<E, T>> right = std::make_unique<LiteralExpr<E, T>>(
            Token(TokenType::NUMBER, "1", 1, 2.0f)  // TODO: put token in object so this doesn't pass
    );

    BinaryExpr<E, T> test_expr(std::move(left), std::move(right), op);
    
    REQUIRE(test_expr.op == Token(TokenType::PLUS, "+"));
    REQUIRE(printer.print(test_expr) == "(+ 1 2)");
}



TEST_CASE("test_print_block_statements", "ast_printer")
{
    ASTPrinter printer;

    std::string test_filename = "test/nesting.lox";
    std::string source;

    try {
        source = read_file(test_filename);
    }
    catch(std::ifstream::failure& e) {
        std::cout << "Failed to read source file " << test_filename << std::endl;
        FAIL();
    }

    Scanner scanner(source);
    Parser parser(scanner.scan());

    auto parsed_output = parser.parse();

    //for(unsigned i = 0; i < parsed_output.size(); ++i)
    //    printer.print(*parsed_output[i].get());
}
