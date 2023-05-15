/*
 * OBJECT
 * Implements a generic object that can transparently hold various things.
 *
 */

#ifndef __OBJECT_HPP
#define __OBJECT_HPP

#include <iomanip>
#include <memory>
#include <optional>
#include <sstream>
#include <variant>

#include "Token.hpp"

constexpr int f_precision = 4;

class Callable;

enum class ObjType {
    NIL,
    NUMBER,
    STRING,
    BOOLEAN,
    FUNCTION,
    CLASS,
};

struct get_type_string
{
    std::string operator()(const std::string& s) const { return "String"; }
    std::string operator()(std::string& s) const { return "String"; }
    std::string operator()(double f) const { return "Number"; }
    std::string operator()(bool b) const { return "Boolean"; }
    std::string operator()(void) const { return "Nil"; }
};


// TODO: what needs to be done to move this definition inside the class?
struct get_value_string
{
    std::string operator()(const std::string& s) const { return s; }
    std::string operator()(double f) const { 
        std::ostringstream oss;
        oss << std::setprecision(f_precision) << f;
        return oss.str();
    }   
    std::string operator()(bool b) const { return std::to_string(b); }
    std::string operator()(void) const { return "Nil"; }
};


/*
 * Generic Object 
 */
struct LoxObject
{
    ObjType type;    // TODO: make these private?
    Token token;
    std::optional<std::variant<std::string, double, bool>> value;
    std::shared_ptr<Callable> callable;
    
    public:
        LoxObject() : type(ObjType::NIL), token(Token()) {}
        LoxObject(const Token& token);

        LoxObject(std::shared_ptr<Callable> c) : type(ObjType::FUNCTION), token(Token()), value(),  callable(std::move(c)) {}

        // Constructors for container types 
        LoxObject(double f) : 
            type(ObjType::NUMBER),
            token(Token(TokenType::NUMBER, std::to_string(f), 0, f)),
            value(f) {}

        LoxObject(const std::string& s) : 
            type(ObjType::STRING),
            token(Token(TokenType::STRING, s, 0, s)),
            value(s) {}

        LoxObject(const TokenType t, const std::string& s) : 
            type(ObjType::STRING),
            token(Token(t)), 
            value(s) {}

        LoxObject(bool b) : 
            type(ObjType::BOOLEAN),
            token(Token(b ? TokenType::TRUE : TokenType::FALSE, std::to_string(b), 0, b)),
            value(b) {}

        bool operator==(const LoxObject& that) const;
        bool operator!=(const LoxObject& that) const;

        std::string get_val_as_str(void) const;
        std::string get_string_val(void) const;
        double      get_double_val(void) const;
        bool        get_bool_val(void) const;
        bool        has_type(void) const;
        std::string get_type_string(void) const;
        bool        has_string_type(void) const;
        bool        has_number_type(void) const;
        bool        has_bool_type(void) const;
        bool        has_callable(void) const;

        std::shared_ptr<Callable> get_callable(void) const;

        std::string to_string(void) const;
        std::string to_repr(void) const;
};



#endif /*__OBJECT_HPP*/
