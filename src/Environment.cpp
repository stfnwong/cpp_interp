/*
 * ENVIRONMENT
 *
 */

#include <iostream>     // TODO: remove this
#include <utility>

#include "Error.hpp"
#include "Lox.hpp"
#include "Environment.hpp"


std::string Environment::level_to_repr(const Environment* env, int level) const
{
    std::ostringstream oss;

    oss << "Vars (level " << level << "): (" << env->values.size() << " vars)"  << std::endl;
    for(const auto& kv : env->values)
        oss << "   [" << kv.first << "] -> " << kv.second.to_string() << std::endl;

    if(env->parent.get())
        oss << this->level_to_repr(env->parent.get(), level+1);

    return oss.str();
}


Environment* Environment::ancestor(int dist)
{
    Environment* env = this;
    for(int i = 0; i < dist; ++i)
    {
        if(env->parent.get())
            env = env->parent.get();
    }

    return env;
}


void Environment::define(const std::string& name, const LoxObject& value)
{
    //this->values.insert(std::make_pair(name, value));
    this->values.insert({name, value});
    //this->values[name] = value;
}


void Environment::define(const Token& name, const LoxObject& value)
{
    this->values.insert({name.lexeme, value});
}

LoxObject Environment::get(const std::string& name)
{
    if(this->values.find(name) != this->values.end())
        return this->values[name];

    if(this->parent)
        return this->parent->get(name);

    // Make a fake token for the runtime error
    throw RuntimeError(Token(TokenType::STRING, name), "undefined variable '" + name + "'.");
}

LoxObject Environment::get(const Token& name)
{
    if(this->values.find(name.lexeme) != this->values.end())
        return this->values[name.lexeme];

    if(this->parent)
        return this->parent->get(name);

    throw RuntimeError(name, "undefined variable '" + name.lexeme + "'.");
}

LoxObject Environment::get_at(int dist, const std::string& name)
{
    return this->ancestor(dist);
}


void Environment::assign(const Token& name, const LoxObject& value)
{
    if(this->values.find(name.lexeme) != this->values.end())
    {
        this->values[name.lexeme] = value;
        return;
    }

    if(this->parent)
    {
        this->parent->assign(name, value);
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



std::string Environment::to_repr(void) const
{
    std::ostringstream oss;
    oss << this->level_to_repr(this, 0);

    return oss.str();
}
