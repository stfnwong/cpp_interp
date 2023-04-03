/*
 * OBJECT
 * Implements a generic object that can transparently hold various things.
 *
 */

#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include <optional>
#include <variant>

#include "Token.hpp"


// Why does this have to be here?
struct get_value_string
{
    std::string operator()(const std::string& s) const { return s; }
    std::string operator()(float f) const { return std::to_string(f); }
    std::string operator()(bool b) const { return std::to_string(b); }
};



struct LoxObject
{
    TokenType src_type;
    std::optional<std::variant<float, bool, std::string>> value;

    // some fancy visitor structure here to get the value..
    struct get_value
    {
        std::string operator()(const std::string& s) const { return s; }
        float operator()(float f) const { return f; }
        bool operator()(bool b) const { return b; }
    };

    public:
        LoxObject() : src_type(TokenType::EMPTY) {}
        LoxObject(const Token& token);

        std::string get_string_val(void) const;
        float       get_float_val(void) const;
        bool        get_bool_val(void) const;
        bool        has_type(void) const;

        std::string to_string(void) const;
};



#endif /*__OBJECT_HPP*/
