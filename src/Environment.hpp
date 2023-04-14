/*
 * ENVIRONMENT
 *
 */

#ifndef __ENVIRONMENT_HPP
#define __ENVIRONMENT_HPP

#include <string>
#include <unordered_map>

#include "Object.hpp"



class Environment
{
    Environment* enclosing;
    // We start with unscoped variables.
    std::unordered_map<std::string, LoxObject> values;

    public:
        Environment() : enclosing(nullptr) {}
        Environment(Environment* enc) : enclosing(enc) {}

        void      define(const std::string& name, const LoxObject& value);
        LoxObject get(const Token& name);
        void      assign(const Token& name, const LoxObject& value);
        //LoxObject get(const std::string& name);
};




#endif /*__ENVIRONMENT_HPP*/
