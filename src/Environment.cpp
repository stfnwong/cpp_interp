/*
 * ENVIRONMENT
 *
 */

#include <iostream>     // TODO: remove this

#include "Error.hpp"
#include "Lox.hpp"
#include "Environment.hpp"


void Environment::define(const std::string& name, const LoxObject& value)
{
    this->values.insert({name, value});
}

LoxObject Environment::get(const Token& name)
{
    std::cout << "[" << __func__ << "] looking for var: [" << name.lexeme << "]" << std::endl;
    if(this->values.find(name.lexeme) != this->values.end())
        return this->values[name.lexeme];

    std::cout << "[" << __func__ << "] var  " << name.lexeme 
        << " not found, checking enclosing..." << std::endl;;

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


std::vector<std::string> Environment::get_vars(void) const
{
    std::vector<std::string> keys;

    for(auto kv: this->values)
        keys.push_back(kv.first);

    return keys;
}
