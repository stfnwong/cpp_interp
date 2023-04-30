/*
 * OBJECT
 * Implements a generic object that can transparently hold various things.
 *
 */

#include "Object.hpp"
#include "Callable.hpp"


LoxObject::LoxObject(const Token& token) : token(token)
{
    switch(token.type)
    {
        case TokenType::TRUE:
            this->value = true;
            this->type = ObjType::BOOLEAN;
            break;

        case TokenType::FALSE:
            this->value = false;
            this->type = ObjType::BOOLEAN;
            break;

        case TokenType::NUMBER:
            this->value = token.get_double_literal();
            this->type = ObjType::NUMBER;
            break;

        case TokenType::STRING:
        case TokenType::IDENTIFIER:
            if(token.has_literal())
                this->value = token.get_string_literal();
            else
                this->value = token.lexeme;
            this->type = ObjType::STRING;
            break;

        default:
            break;
    }
}



// ======== OPERATORS ======== //
bool LoxObject::operator==(const LoxObject& that) const
{
    if(this->token.type != that.token.type)
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


double LoxObject::get_double_val(void) const
{
    if(this->value.has_value())
        return std::get<double>(this->value.value());

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

TokenType LoxObject::get_type(void) const
{
    return this->token.type;
}

std::string LoxObject::get_type_string(void) const
{
    if(this->is_callable())
        return this->callable->get_type_string();

    //if(this->has_type())
    //    return std::visit(get_type_string(), this->value.value());

    return "Nil";
}

bool LoxObject::has_string_type(void) const
{
    return (this->token.type == TokenType::STRING || this->token.type == TokenType::IDENTIFIER) ? true : false;
}

bool LoxObject::has_number_type(void) const
{
    return (this->type == ObjType::NUMBER) ? true : false;
    //return (this->token.type == TokenType::NUMBER) ? true : false;
}

bool LoxObject::is_callable(void) const
{
    return (this->type == ObjType::FUNCTION) ? true : false;
}

std::string LoxObject::to_string(void) const
{
    if(this->has_type())
        return std::visit(get_value_string(), this->value.value());

    return "Nil";
}

std::string LoxObject::to_repr(void) const
{
    std::ostringstream oss;
    if(this->has_string_type())
        oss << "LoxObject<\"" << this->token.to_string() << "\">";
    else
        oss << "LoxObject<" << this->token.to_string() << ">";

    return oss.str();
}
