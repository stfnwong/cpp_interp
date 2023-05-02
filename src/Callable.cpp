/*
 * CALLABLE
 * Interfaces for callables
 *
 */

#include <iostream>     // TODO: remove, debug only

#include "Callable.hpp"
#include "Environment.hpp"



LoxFunction::LoxFunction(FunctionStmt<EType, VType>* d) : 
    Callable(CallableType::FUNCTION), decl(d)
{
}

LoxObject LoxFunction::call(Interpreter& interp, const std::vector<LoxObject>& args)
{
    //Environment env(interp.get_globals());
    Environment env = interp.get_globals();

    std::cout << "[" << __func__ << "] env: " << std::endl << env.to_repr() << std::endl;

    for(unsigned i = 0; i < this->decl->params.size(); ++i)
        env.define(decl->params[i].lexeme, args[i]);

    try {
        interp.execute_block(decl->body, env);
    }
    catch(Return& rv) {
        return rv.value;
    }

    return LoxObject();         // bogus return
}


unsigned LoxFunction::arity(void) const 
{
    return this->decl->params.size();
}

std::string LoxFunction::to_string(void) const
{
    return "Function<" + this->decl->name.lexeme + ">";
}

std::string LoxFunction::name(void) const
{
    return this->decl->name.lexeme;
}

std::string LoxFunction::get_type_string(void) const
{
    return "Function";
}
