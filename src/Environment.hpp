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
    // We start with unscoped variables.
    std::unordered_map<std::string, LoxObject> values;

    public:
        Environment() {}

        void define(const std::string& name, const LoxObject& value);
        LoxObject get(const Token& name);
        //LoxObject get(const std::string& name);
};




#endif /*__ENVIRONMENT_HPP*/
