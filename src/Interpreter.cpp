/*
 * INTERPRETER
 */


#include "Interpreter.hpp"

static bool had_runtime_error = false;

static void runtime_error(RuntimeError& e)
{
    std::cerr << e.what() << std::endl << "[line " << e.token.line << "]";
    had_runtime_error = true;
}


bool Interpreter::is_truthy(const LoxObject& obj) const
{
    switch(obj.type)
    {
        case TokenType::NIL:
            return false;
        case TokenType::TRUE:
            return true;
        case TokenType::FALSE:
            return false;
        default:
            return true;
    }
}

bool Interpreter::is_equal(const LoxObject& a, const LoxObject& b) const
{
    if(!a.has_type() && !b.has_type())
        return true;
    if(!a.has_type())
        return false;

    return a == b;
}


void Interpreter::check_number_operand(const Token& otor, const LoxObject& orand)
{
    if(orand.has_number_type())
        return;

    throw RuntimeError(otor, "Operand must be a number");
}


void Interpreter::check_number_operands(const Token& otor, const LoxObject& o1, const LoxObject& o2)
{
    if(o1.has_number_type() && o2.has_number_type())
        return;

    throw RuntimeError(otor, "Both operands must be numbers");
}

// TODO: this has to be a pointer...
LoxObject Interpreter::evaluate(const std::unique_ptr<Expr<E, T>>& expr)
{
    return expr->accept(*this);
}


// ======== EXPRESSION VISITOR FUNCTIONS ======== //
LoxObject Interpreter::visit(LiteralExpr<E, T>& expr)
{
    return expr.value;
}

LoxObject Interpreter::visit(GroupingExpr<E, T>& expr)
{
    return this->evaluate(expr.left);
}

LoxObject Interpreter::visit(UnaryExpr<E, T>& expr)
{
    LoxObject right = this->evaluate(expr.right);

    switch(expr.op.type)
    {
        case TokenType::MINUS:
            return LoxObject(-right.get_float_val());
        case TokenType::BANG:
            return !this->is_truthy(right);
        default:
            break;          // shut linter up
    }

    throw RuntimeError(expr.op, "Invalid operator for Unary Expression");
    return LoxObject();     // unreachable - TODO: should we throw an error here?
}

LoxObject Interpreter::visit(BinaryExpr<E, T>& expr)
{
    LoxObject left = this->evaluate(expr.left);
    LoxObject right = this->evaluate(expr.right);

    switch(expr.op.type)
    {
        // Arithmetic operators 
        case TokenType::MINUS:
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_float_val() - right.get_float_val());

        case TokenType::SLASH:
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_float_val() / right.get_float_val());

        case TokenType::STAR:
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_float_val() * right.get_float_val());

        // Lox allows string concat with '+' operator 
        case TokenType::PLUS:
            this->check_number_operands(expr.op, left, right);
            if(left.has_string_type() && right.has_string_type())
                return LoxObject(left.get_string_val() + right.get_string_val());
            if(left.has_number_type() && right.has_number_type())
                return LoxObject(left.get_float_val() + right.get_float_val());

            throw RuntimeError(expr.op, "Operands must be two numbers or two strings");
            break;

        // Comparison operators - these only support number types
        case TokenType::GREATER:
            this->check_number_operands(expr.op, left, right);
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_float_val() > right.get_float_val());

        case TokenType::GREATER_EQUAL:
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_float_val() >= right.get_float_val());

        case TokenType::LESS:
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_float_val() >= right.get_float_val());

        case TokenType::LESS_EQUAL:
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_float_val() <= right.get_float_val());

        default:
            break;      // shut linter up
    }

    return LoxObject();     // TODO: unreachable, throw error?
}


// ======== PUBLIC FUNCTIONS ======== //
std::string Interpreter::interpret(const std::unique_ptr<Expr<E, T>>& expr)
{
    try {
        LoxObject value = this->evaluate(expr);
        return value.to_string();
    }
    catch(RuntimeError& e) {
        runtime_error(e);
    }

    return "FAILED";  // unreachable, but shuts linter up
}
