/*
 * OBJECT
 * Implements a generic object that can transparently hold various things.
 *
 */

#include "Object.hpp"


LoxObject::LoxObject(const Token& token) : type(token.type)
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


bool LoxObject::operator==(const LoxObject& that) const
{
    if(this->type != that.type)
        return false;
    if(this->value.has_value() != that.value.has_value())
        return false;
    else
        return this->value.value() == that.value.value();

    return true;
}


bool LoxObject::operator!=(const LoxObject& that) const
{
    return !(*this == that);
}


std::string LoxObject::get_val_as_str(void) const
{
    if(this->value.has_value())
        return std::visit(get_value_string(), this->value.value());

    return "Nil";
}

// TODO: do I want to throw here? 
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

bool LoxObject::has_string_type(void) const
{
    return (this->type == TokenType::STRING || this->type == TokenType::IDENTIFIER) ? true : false;
}

bool LoxObject::has_number_type(void) const
{
    return (this->type == TokenType::NUMBER) ? true : false;
}

std::string LoxObject::to_string(void) const
{
    if(this->has_type())
        return std::visit(get_value_string(), this->value.value());

    return "LoxObject()";
}

std::string LoxObject::to_repr(void) const
{
    std::ostringstream oss;
    if(this->value.has_value())
        oss << "LoxObject<" << std::visit(get_value_string(), this->value.value()) << ">";
    else
        oss << "LoxObject<Null>";

    return oss.str();
}
