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
    // We start with unscoped variables.
    std::shared_ptr<Environment> enclosing;   
    std::unordered_map<std::string, LoxObject> values;

    public:
        Environment() : enclosing(nullptr) {}
        Environment(std::shared_ptr<Environment> enc) : enclosing(enc) {}
        //Environment(std::shared_ptr<Environment> enc) : enclosing(std::move(enc)) {}

        // Copy assignment 
        //Environment& operator=(const Environment& that) ;


        void      define(const std::string& name, const LoxObject& value);
        void      define(const Token& name, const LoxObject& value);
        LoxObject get(const std::string& name); 
        LoxObject get(const Token& name);
        void      assign(const Token& name, const LoxObject& value);
        bool      has_outer(void) const; // TODO: get rid of this...
        std::vector<std::string> get_vars(void) const;
};




#endif /*__ENVIRONMENT_HPP*/
