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
    std::string operator()(double f) const { 
        std::ostringstream oss;
        oss << std::setprecision(f_precision) << f;
        return oss.str();
    }   
    std::string operator()(bool b) const { return std::to_string(b); }
};



struct LoxObject
{
    Token token;
    std::optional<std::variant<double, bool, std::string>> value;

    // some fancy visitor structure here to get the value..
    struct get_value
    {
        std::string operator()(const std::string& s) const { return s; }
        double operator()(double f) const { return f; }
        bool operator()(bool b) const { return b; }
    };

    public:
        LoxObject() : token(Token()) {}
        LoxObject(const Token& token);

        // Constructors for container types 
        LoxObject(double f) : 
            token(Token(TokenType::NUMBER, std::to_string(f), 0, f)),
            value(f) {}

        LoxObject(const std::string& s) : 
            token(Token(TokenType::STRING, s, 0, s)),
            value(s) {}

        LoxObject(const TokenType t, const std::string& s) : 
            token(Token(t)), 
            value(s) {} 

        LoxObject(bool b) : 
            token(Token(b ? TokenType::TRUE : TokenType::FALSE, std::to_string(b), 0, b)),
            value(b) {} 

        bool operator==(const LoxObject& that) const;
        bool operator!=(const LoxObject& that) const;

        std::string get_val_as_str(void) const;
        std::string get_string_val(void) const;
        double      get_double_val(void) const;
        bool        get_bool_val(void) const;
        bool        has_type(void) const;
        TokenType   get_type(void) const;
        bool        has_string_type(void) const;
        bool        has_number_type(void) const;

        std::string to_string(void) const;
        std::string to_repr(void) const;
};



#endif /*__OBJECT_HPP*/
