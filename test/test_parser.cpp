/*
 * PARSER
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include <iostream>
#include <vector>

#include "Parser.hpp"
#include "Common.hpp"    // includes Object.hpp
#include "Scanner.hpp"
#include "Util.hpp"


TEST_CASE("test_parse_empty_input", "parser")
{
    std::vector<Token> empty_input;
    Parser parser(empty_input);

    auto statements = parser.parse();
    REQUIRE(statements.size() == 0);
}

TEST_CASE("test_parse_binary_expr", "parser")
{
    // expression is "2 + 2"
    std::vector<Token> test_tokens = {
        Token(TokenType::NUMBER, "2", 1, 2.0f),
        Token(TokenType::PLUS, "+", 1),
        Token(TokenType::NUMBER, "2", 1, 2.0f),
        Token(TokenType::SEMICOLON, ";", 1),
        Token(TokenType::LOX_EOF, "", 1),
    };
    Parser parser(test_tokens);
    REQUIRE(parser.num_tokens() == test_tokens.size());
    std::string exp_string_repr = "ExpressionStmt<2 + 2>";

    // Should return an expression
    auto parse_result = parser.parse();
    REQUIRE(parse_result.size() == 1);
    REQUIRE(parse_result[0]->to_string() == exp_string_repr);
}


TEST_CASE("test_parse_block_statement", "parser")
{
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
    // 15 tokens?   6 + 9 tokens?

    unsigned num_exprs = 0;
    for(unsigned i = 0; i < parsed_output.size(); ++i)
    {
        std::cout << i << " " << parsed_output[i].get()->to_string();
        if(parsed_output[i]->get_type() == StmtType::BLOCK)
        {
            std::cout << " " << typeid(parsed_output[i]).name() << std::endl;
            auto ptr = static_cast<BlockStmt<EType, VType>*>(parsed_output[i].get());
            //for(unsigned j = 0; j < parsed_output[i]->size(); ++j)
            for(unsigned j = 0; j < ptr->size(); ++j)
            {
                std::cout << i << "," << j << " " << ptr->statements[j]->to_string() << std::endl;
                num_exprs++;
            }
        }
        else
            num_exprs++;

        std::cout << std::endl;
    }

    std::cout << "There were " << num_exprs << " expressions" << std::endl;
}
