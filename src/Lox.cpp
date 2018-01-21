/* LOX CLASS
 * Since I can't be bothered with Java, I am going to try and write 
 * alot of the first part of the book in C++
 *
 * Stefan Wong 2018
 */

#include <cstdio>
#include <fstream>
#include <algorithm>
#include <iterator>

#include "Lox.hpp"

Lox::Lox()
{
    // TODO : stuff 
}

int Lox::runFile(const std::string &path)
{
     // Read bytes from file 
    std::ifstream infile;
    size_t num_bytes;

    infile.open(path);
    if(!infile)
    {
        fprintf(stderr, "Unable to open file %s\n", path);
        return -1;
    }
    // Read all data into memory 
    infile.seekg(0, infile.end);
    num_bytes = infile.tellg();
    infile.seekg(0, infile.beg);

    std::vector<uint8_t> source_file;
    source_file.reserve(num_bytes);
    std::copy(std::istream_iterator<uint8_t>(infile),
            std::istream_iterator<uint8_t>(),
            std::back_inserter(source_file));

    this->lox_run(source_file);
}

void Lox::runPrompt(void)
{
    // TODO : need some kind of interactive version here ...
}

int Lox::lox_main(const std::vector<std::string> &args)
{
    if(args.size() > 1)
    {
        fprintf(stdout, "Usage: lox [script]\n");
        return -1;
    }
    else if (args.size() == 1)
        this->runFile(args[0]);
    else
        this->runPrompt();
}

void Lox::lox_run(const std::string &source)
{

}
