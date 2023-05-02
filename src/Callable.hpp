/*
 * CALLABLE
 * Interfaces for callables
 *
 */


#ifndef __CALLABLE_HPP
#define __CALLABLE_HPP

#include <string>
#include <vector>

#include "Common.hpp"
#include "Interpreter.hpp"
#include "Object.hpp"


enum class CallableType { FUNCTION, CLASS };


/*
 * ABC for a callable
 */
class Callable
{
    CallableType type;

    protected:
        explicit Callable(CallableType t) : type(t) {}

    public:
        virtual ~Callable() = default;
        virtual LoxObject   call(Interpreter& interpreter, const std::vector<LoxObject>& args) = 0;
        virtual unsigned    arity(void) const = 0;
        virtual std::string to_string(void) const = 0;
        virtual std::string name(void) const = 0;
        virtual std::string get_type_string(void) const = 0;
};



class LoxFunction : public Callable
{
   //std::unique_ptr<FunctionStmt<EType, VType>> decl; // NOTE: who owns this?
   FunctionStmt<EType, VType>* decl;
   //std::shared_ptr<Environment> closure;

   public:
        LoxFunction(FunctionStmt<EType, VType>* d);
        ~LoxFunction() {} 

        LoxObject   call(Interpreter& interp, const std::vector<LoxObject>& args) override;
        unsigned    arity(void) const override;
        std::string to_string(void) const override;
        std::string name(void) const override;
        std::string get_type_string(void) const override;
};


#endif /*__CALLABLE_HPP*/
