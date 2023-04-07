/*
 * OBJECT
 * Implements a generic object that can transparently hold various things.
 *
 */

#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include <iomanip>
#include <optional>
#include <sstream>
#include <variant>

#include "Token.hpp"

constexpr int f_precision = 4;

// Why does this have to be here?
struct get_value_string
{
    std::string operator()(const std::string& s) const { return s; }
    std::string operator()(float f) const { 
        std::ostringstream oss;
        oss << std::setprecision(f_precision) << f;
        return oss.str();
    }   
    std::string operator()(bool b) const { return std::to_string(b); }
};



struct LoxObject
{
    TokenType type;
    std::optional<std::variant<float, bool, std::string>> value;

    // some fancy visitor structure here to get the value..
    struct get_value
    {
        std::string operator()(const std::string& s) const { return s; }
        float operator()(float f) const { return f; }
        bool operator()(bool b) const { return b; }
    };

    public:
        LoxObject() : type(TokenType::EMPTY) {}
        LoxObject(const Token& token);

        bool operator==(const LoxObject& that) const;
        bool operator!=(const LoxObject& that) const;

        std::string get_val_as_str(void) const;
        std::string get_string_val(void) const;
        float       get_float_val(void) const;
        bool        get_bool_val(void) const;
        bool        has_type(void) const;

        std::string to_string(void) const;
        std::string to_repr(void) const;
};



#endif /*__OBJECT_HPP*/
