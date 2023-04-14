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


static const std::string VERSION_STRING = "deez nuts";
static bool had_error = false;


void run(const std::string& source)
{
    Scanner scanner(source);
    auto scanned_tokens = scanner.scan();
    for(const auto& tok : scanned_tokens)
        std::cout << tok.to_repr() << std::endl;
    Parser parser(scanned_tokens);

    const auto statements = parser.parse();
    if(Lox::had_error)
        return;

    Interpreter interp;
    interp.interpret(statements);
}


void run_file(const std::string& filename)
{
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

    run(source);

    if(Lox::had_error)
        exit(2);
    if(Lox::had_runtime_error)
        exit(3);
}


void run_prompt(void)
{
    std::cout << "Lox version [" << VERSION_STRING << "]" << std::endl;

    std::string code;

    while(1)
    {
        std::cout << "> ";
        if(std::getline(std::cin, code))
        {
            run(code);
            Lox::had_error = false;  // why do I have to do this?
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
