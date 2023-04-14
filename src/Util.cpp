/*
 * UTIL
 * The stuff that doesn't go anywhere else
 */

#include <fstream>
#include "Util.hpp"


std::string read_file(const std::string& filename)
{
    std::ifstream file(filename);

    if(!file.good()) 
    {
        return "";
    }

    std::string line;
    std::string source;

    while(std::getline(file, line))
        source += line + "\n";

    return source;
}
