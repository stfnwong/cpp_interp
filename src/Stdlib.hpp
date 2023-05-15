/*
 * STDLIB
 * Library Functions
 */


#ifndef __STDLIB_HPP
#define __STDLIB_HPP

#include "Callable.hpp"



class StdClock : public Callable
{
    public:
        StdClock();

        LoxObject   call(Interpreter& interp, const std::vector<LoxObject>& args) override;
        unsigned    arity(void) const override;
        std::string to_string(void) const override;
        std::string name(void) const override;
        std::string get_type_string(void) const override;
};



#endif /*__STDLIB_HPP*/
