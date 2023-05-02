/* LOX CLASS
 * Since I can't be bothered with Java, I am going to try and write 
 * alot of the first part of the book in C++
 *
 * Stefan Wong 2018
 */

#include <fstream>
#include <algorithm>
#include <iterator>
#include <iostream>


#include "Scanner.hpp"
#include "Parser.hpp"
#include "Interpreter.hpp"
#include "Lox.hpp"

#include "ASTPrinter.hpp"


static constexpr bool DEBUG_OUTPUT = true;      // TODO: make cmdline option for this

static const std::string VERSION_STRING = "deez nuts";
static bool had_error = false;


void run(const std::string& source, Interpreter& interp)
{
    Scanner scanner(source);
    auto scanned_tokens = scanner.scan();
    Parser parser(scanned_tokens);

    if(DEBUG_OUTPUT)
    {
        std::cout << "Scanner produced the following tokens:" << std::endl;
        for(unsigned i = 0; i < scanned_tokens.size(); ++i)
            std::cout << " " << i << ": " << scanned_tokens[i].to_repr() << std::endl;
    }

    const auto statements = parser.parse();
    if(Lox::had_error)
        return;

    if(DEBUG_OUTPUT)
    {
        std::cout << "Parser produced the following statements:" << std::endl;
        for(unsigned i = 0; i < statements.size(); ++i)
            std::cout << " " << i << ": " << statements[i]->to_string() << std::endl;

        ASTPrinter printer;
        std::cout << "AST for output:" << std::endl;
        for(unsigned i = 0; i < statements.size(); ++i)
            std::cout << " " << i << ": " << printer.print(*statements[i].get()) << std::endl;
    }

    interp.interpret(statements);
}


void run_file(const std::string& filename)
{
    Interpreter interp;
    std::ifstream file(filename);

    if(!file.good()) 
    {
        std::cerr << "Failed to open file [" << filename 
            << "] no such file or directory." << std::endl;
        return;
    }

    std::string line;
    std::string source;

    while(std::getline(file, line))
        source += line + "\n";

    run(source, interp);

    if(Lox::had_error)
        exit(2);
    if(Lox::had_runtime_error)
        exit(3);
}


void run_prompt(void)
{
    std::cout << "Lox version [" << VERSION_STRING << "]" << std::endl;

    Interpreter interp;
    std::string code;

    while(1)
    {
        std::cout << "> ";
        if(std::getline(std::cin, code))
        {
            std::cout << "[" << __func__ << "] interp env before: " << std::endl;
            std::cout << interp.get_globals().to_repr() << std::endl;
            run(code, interp);    // TODO: segfault referring to previous env...
            Lox::had_error = false;  // why do I have to do this?
            std::cout << "[" << __func__ << "] interp env after: " << std::endl;
            std::cout << interp.get_globals().to_repr() << std::endl;
        }
        else
        {
            std::cout << std::endl;
            break;
        }
    }
}


int main(int argc, char *argv[])
{
    if(argc > 2)
    {
        std::cout << "usage: lox [filename]" << std::endl;
        exit(1);
    }
    else if(argc == 2)
        run_file(argv[1]);
    else
        run_prompt();

    return 0;
}
