/*
 * STDLIB
 * Library Functions
 */

#include <chrono>

#include "Stdlib.hpp"


StdClock::StdClock() : Callable(CallableType::FUNCTION) {} 

unsigned StdClock::arity(void) const 
{
    return 0;
}

LoxObject StdClock::call(Interpreter& interp, const std::vector<LoxObject>& args)
{
    double ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();

    return LoxObject(ms);
}


std::string StdClock::to_string(void) const 
{
    return "<library fn>";
}

std::string StdClock::name(void) const
{
    return "clock";
}

std::string StdClock::get_type_string(void) const
{
    return "Function";
}
