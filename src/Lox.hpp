/* LOX CLASS
 * Since I can't be bothered with Java, I am going to try and write 
 * alot of the first part of the book in C++
 *
 * Stefan Wong 2018
 */


#ifndef __LOX_HPP
#define __LOX_HPP

#include <string>
#include <vector>


class Lox
{
    protected:    

    public:
        Lox();

        int runFile(const std::string &path);
        void runPrompt(void);
        int lox_main(const std::vector<std::string> &args);
        //void lox_run(const std::string &source);
        void lox_run(const unsigned char *source);
        unsigned char* read_file(const std::string &fname, int *num_bytes);
};

#endif /*__LOX_HPP*/
