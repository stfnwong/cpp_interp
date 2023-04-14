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

    if(this->enclosing)
        return this->enclosing->get(name);

    throw RuntimeError(name, "undefined variable '" + name.lexeme + "'.");
}

void Environment::assign(const Token& name, const LoxObject& value)
{
    if(this->values.find(name.lexeme) != this->values.end())
    {
        this->values[name.lexeme] = value;
        return;
    }

    if(this->enclosing)
    {
        this->enclosing->assign(name, value);
        return;
    }

    throw RuntimeError(name, "undefined variable '" + name.lexeme + "'.");
}
