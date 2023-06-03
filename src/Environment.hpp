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
    std::shared_ptr<Environment> parent;   
    std::unordered_map<std::string, LoxObject> values;

    private:
        std::string level_to_repr(const Environment* env, int level) const;
        Environment* ancestor(int dist);

    public:
        Environment() : parent(nullptr) {}
        Environment(const Environment& env) : 
            parent(std::make_shared<Environment>(env.parent)), values(env.values) {} 
        Environment(std::shared_ptr<Environment> enc) : parent(enc) {}

        // TODO: testing copy assignment

        void        define(const std::string& name, const LoxObject& value);
        void        define(const Token& name, const LoxObject& value);
        LoxObject   get(const std::string& name); 
        LoxObject   get(const Token& name);
        LoxObject   get_at(int dist, const std::string& name);
        void        assign(const Token& name, const LoxObject& value);
        // TODO: this is also more a debugging function... get rid of it after resolver is complete
        std::vector<std::string> get_vars(void) const;
        // TODO: remove, debug only
        std::string to_repr(void) const;
};




#endif /*__ENVIRONMENT_HPP*/
