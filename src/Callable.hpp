/*
 * CALLABLE
 * Interfaces for callables
 *
 */


#ifndef __CALLABLE_HPP
#define __CALLABLE_HPP

#include <string>
#include <vector>

#include "Interpreter.hpp"
#include "Object.hpp"


enum class CallableType { FUNCTION, CLASS };


class Callable
{
    CallableType type;

    public:
        Callable(CallableType t) : type(t) {}
        virtual ~Callable() = default;
        virtual LoxObject call(Interpreter& interpreter, const std::vector<LoxObject>& args);
        virtual int arity(void) const = 0;
        virtual std::string to_string(void) const = 0;
        virtual std::string get_type_string(void) const = 0;
};


#endif /*__CALLABLE_HPP*/
