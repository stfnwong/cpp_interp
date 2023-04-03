/*
 * OBJECT
 * Implements a generic object that can transparently hold various things.
 *
 */

#include <sstream>
#include "Object.hpp"


LoxObject::LoxObject(const Token& token) : src_type(token.type)
{
    switch(token.type)
    {
        case TokenType::TRUE:
            this->value = true;
            break;

        case TokenType::FALSE:
            this->value = false;
            break;

        case TokenType::NUMBER:
            this->value = token.get_float_literal();
            break;

        case TokenType::STRING:
        case TokenType::IDENTIFIER:
            this->value = token.get_string_literal();
            break;

        default:
            break;
    }
}

std::string LoxObject::get_string_val(void) const
{
    if(this->value.has_value())
        return std::get<std::string>(this->value.value());

    return "";
}


float LoxObject::get_float_val(void) const
{
    if(this->value.has_value())
        return std::get<float>(this->value.value());

    return 0;
}

bool LoxObject::get_bool_val(void) const
{
    if(this->value.has_value())
        return std::get<bool>(this->value.value());
    return false;
}


bool LoxObject::has_type(void) const
{
    return (this->value.has_value()) ? true : false;
}


std::string LoxObject::to_string(void) const
{
    std::ostringstream oss;
    if(this->value.has_value())
        oss << "LoxObject<" << std::visit(get_value_string(), this->value.value()) << ">";
    else
        oss << "LoxObject<Null>";

    return oss.str();
}