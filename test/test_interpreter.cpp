/*
 * INTERPRETER
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include "Interpreter.hpp"
#include "Parser.hpp"
#include "Scanner.hpp"
#include "Util.hpp"


std::vector<std::unique_ptr<Stmt<EType, VType>>> parse_source(const std::string& source)
{
    Scanner scanner(source);
    Parser parser(scanner.scan());
    return parser.parse();
}

std::string read_source_or_fail(const std::string& filename)
{
    std::string source;
    try {
        source = read_file(filename);
        std::cout << "Read source file [" << filename << "]" << std::endl;
    }
    catch(std::ifstream::failure& e) {
        std::cout << "Failed to read source file [" << filename << "]" << std::endl;
        FAIL();
    }

    return source;
}



TEST_CASE("test_interpret_ops", "interpreter")
{
    std::string filename = "test/ops.lox";
    std::string source = read_source_or_fail(filename);

    auto parsed_output = parse_source(source);
    REQUIRE(parsed_output.size() == 4);

    Interpreter interp;

    LoxObject res;

    // var a = 1 + 2;
    res = interp.execute(parsed_output[0]);
    REQUIRE(res.has_type() == true);
    REQUIRE(res.has_number_type() == true);
    REQUIRE(double_equal(res.get_double_val(), 3.0));

    // var b = 1 < 2
    res = interp.execute(parsed_output[1]);
    REQUIRE(res.has_type() == true);
    REQUIRE(res.has_bool_type() == true);
    REQUIRE(res.get_bool_val() == true);

    // var b = 1 <= 100
    res = interp.execute(parsed_output[2]);
    REQUIRE(res.has_type() == true);
    REQUIRE(res.has_bool_type() == true);
    REQUIRE(res.get_bool_val() == false);

    // var b = 100 > 100;
    res = interp.execute(parsed_output[3]);
    REQUIRE(res.has_type() == true);
    REQUIRE(res.has_bool_type() == true);
    REQUIRE(res.get_bool_val() == false);

}


TEST_CASE("test_parse_and_interpret", "interpreter")
{
    std::string source = "2+2;";
    auto parsed_output = parse_source(source);

    Interpreter interp;

    LoxObject res = interp.execute(parsed_output[0]);
    REQUIRE(res.has_type() == true);
    REQUIRE(res.has_number_type() == true);
    REQUIRE(double_equal(res.get_double_val(), 4.0));
}

TEST_CASE("test_interpret_block_statements", "interpreter")
{
    const std::string nesting_filename = "test/nesting.lox";
    std::string source = read_source_or_fail(nesting_filename);

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



//TEST_CASE("test_interpret_fib_rec", "interpreter")
//{
//    const std::string nesting_filename = "test/fib_rec.lox";
//    std::string source;
//    try {
//        source = read_file(nesting_filename);
//        std::cout << "Read source file [" << nesting_filename << "]" << std::endl;
//    }
//    catch(std::ifstream::failure& e) {
//        std::cout << "Failed to read source file " << nesting_filename << std::endl;
//        FAIL();
//    }
//
//    Scanner scanner(source);
//    Parser parser(scanner.scan());
//
//    auto statements = parser.parse();
//    REQUIRE(statements.size() == 2);
//    //std::cout << "Parser produced " << statements.size() << " statements." << std::endl;
//    //for(unsigned i = 0; i < statements.size(); ++i)
//    //    std::cout << i << ": " << statements[i]->to_string() << std::endl;
//
//    Interpreter test_interp;
//
//    // fib(4) = 3
//    // fib(8) = 21
//    LoxObject res;
//    res = test_interp.execute(statements[0]);  // this should be empty...
//    res = test_interp.execute(statements[1]);
//    //std::cout << "result: " <<  res.to_string() << std::endl;
//
//    REQUIRE(res.has_type() == true);
//    REQUIRE(res.has_number_type() == true);
//    REQUIRE(double_equal(res.get_double_val(), 3.0));
//}
