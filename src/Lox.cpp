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

#include <cstdio>

#include "Lox.hpp"

// Routine to read files, C style 
unsigned char* Lox::read_file(const std::string &fname, int *num_bytes)
{
    FILE *fp;
    size_t length;
    unsigned char *source;

    fp = fopen(fname.c_str(), "rb");
    if(!fp)
    {
        fprintf(stderr, "Failed to read file %s\n", fname.c_str());
        *num_bytes = -1;
        return nullptr;
    }

    fseek(fp, 0, SEEK_END);
    length = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    source = new unsigned char[length];
    if(!source)
    {
        fprintf(stderr, "Failed to allocate memory for source\n");
        *num_bytes = -1;
        return nullptr;
    }

    size_t idx = 0;
    unsigned char c;
    do
    {
        c = fread(source, sizeof(unsigned char), 1, fp);
        if(c == EOF)
            break;
        idx++;
    }while(idx < length);
    source[length] = '\0';
    *num_bytes = length;

    return source;
}


Lox::Lox()
{
    // TODO : stuff 
}

int Lox::runFile(const std::string &path)
{
     // Read bytes from file 
    std::ifstream infile;
    int num_bytes;

    infile.open(path);
    if(!infile)
    {
        fprintf(stderr, "Unable to open file %s\n", path.c_str());
        return -1;
    }
    // Read all data into memory 
    infile.seekg(0, infile.end);
    num_bytes = infile.tellg();
    infile.seekg(0, infile.beg);

    //std::vector<uint8_t> source_file;
    //source_file.reserve(num_bytes);
    //std::copy(std::istream_iterator<uint8_t>(infile),
    //        std::istream_iterator<uint8_t>(),
    //        std::back_inserter(source_file));

    unsigned char *source_file;
    source_file = this->read_file(path, &num_bytes);
    if(source_file == nullptr)
    {
        fprintf(stderr, "Failed to read source file %s\n", path.c_str());
    }

    this->lox_run(source_file);

    delete[] source_file;
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

void Lox::lox_run(const unsigned char *source)
{

}
