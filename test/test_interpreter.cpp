/*
 * INTERPRETER
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include "Interpreter.hpp"
#include "Parser.hpp"
#include "Scanner.hpp"
#include "Util.hpp"


//TEST_CASE("test_interpret_binary_expr", "interpreter")
//{
//    // Expression is 1 + 1 
//    Token op(TokenType::PLUS, "+");
//    
//    std::unique_ptr<LiteralExpr<E, T>> left = std::make_unique<LiteralExpr<E, T>>(
//            Token(TokenType::NUMBER, "1", 1, 1.0)
//    );
//
//    std::unique_ptr<LiteralExpr<E, T>> right = std::make_unique<LiteralExpr<E, T>>(
//            Token(TokenType::NUMBER, "1", 1, 1.0)
//    );
//
//    auto expr_ptr = std::make_unique<BinaryExpr<E, T>>(std::move(left), std::move(right), op);
//    
//    Interpreter interp;
//    std::string out_eval = interp.interpret(std::move(expr_ptr));
//    std::string exp_eval_str = "2";
//
//    REQUIRE(out_eval == exp_eval_str);
//}


TEST_CASE("test_parse_and_interpret", "interpreter")
{
    std::vector<Token> test_tokens = {
        Token(TokenType::NUMBER, "2", 1, 2.0f),
        Token(TokenType::PLUS, "+", 1),
        Token(TokenType::NUMBER, "2", 1, 2.0f),
        Token(TokenType::SEMICOLON, ";", 1),
        Token(TokenType::LOX_EOF, "", 1),
    };
    Parser parser(test_tokens);
    REQUIRE(parser.num_tokens() == test_tokens.size());

    auto parsed_output = parser.parse();
    REQUIRE(parsed_output.size() == 1);

    Interpreter interp;

    interp.interpret(parsed_output);
    LoxObject res = interp.execute(parsed_output[0]);
    REQUIRE(res.has_type() == true);
    REQUIRE(res.has_number_type() == true);
    REQUIRE(double_equal(res.get_double_val(), 4.0));
}

TEST_CASE("test_interpret_block_statements", "interpreter")
{
    const std::string nesting_filename = "test/nesting.lox";
    std::string source;
    try {
        source = read_file(nesting_filename);
        std::cout << "Read source file [" << nesting_filename << "]" << std::endl;
    }
    catch(std::ifstream::failure& e) {
        std::cout << "Failed to read source file " << nesting_filename << std::endl;
        FAIL();
    }

    Scanner scanner(source);
    Parser parser(scanner.scan());

    auto statements = parser.parse();
    std::cout << "Parser produced " << statements.size() << " statements." << std::endl;
    Interpreter test_interp;

    for(unsigned i = 0; i < statements.size(); ++i)
    {
        LoxObject res = test_interp.execute(statements[i]);
        std::cout << "[" << i << "] : " << res.to_string() << std::endl;
    }
    
    //for(const auto& stmt : statements)
    //{
    //    LoxObject res = test_interp.execute(stmt);
    //    std::cout << res.to_string() << std::endl;
    //}


    //test_interp.interpret(statements);
}
