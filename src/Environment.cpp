/*
 * ENVIRONMENT
 *
 */

#include "Error.hpp"
#include "Lox.hpp"
#include "Environment.hpp"


void Environment::define(const std::string& name, const LoxObject& value)
{
    this->values.insert({name, value});
}

LoxObject Environment::get(const Token& name)
{
    if(this->values.find(name.lexeme) != this->values.end())
        return this->values[name.lexeme];

    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}
