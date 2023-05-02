/*
 * ENVIRONMENT
 *
 */

#ifndef __ENVIRONMENT_HPP
#define __ENVIRONMENT_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Object.hpp"



class Environment
{
    //Environment* enclosing;
    std::shared_ptr<Environment> enclosing;   
    std::unordered_map<std::string, LoxObject> values;

    public:
        Environment() : enclosing(nullptr) {}
        Environment(const Environment& env) : 
            enclosing(std::make_shared<Environment>(env.enclosing)), values(env.values) {} 
        Environment(std::shared_ptr<Environment> enc) : enclosing(enc) {}

        // TODO: testing copy assignment

        void      define(const std::string& name, const LoxObject& value);
        void      define(const Token& name, const LoxObject& value);
        LoxObject get(const std::string& name); 
        LoxObject get(const Token& name);
        void      assign(const Token& name, const LoxObject& value);
        std::vector<std::string> get_vars(void) const;
        // TODO: remove, debug only
        std::string to_repr(void) const;
};




#endif /*__ENVIRONMENT_HPP*/
