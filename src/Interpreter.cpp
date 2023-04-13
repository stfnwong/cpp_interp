/*
 * INTERPRETER
 */


#include <iostream>
#include "Interpreter.hpp"


static bool had_runtime_error = false;

static void runtime_error(RuntimeError& e)
{
    std::cerr << e.what() << std::endl << "[line " << e.token.line << "]";
    had_runtime_error = true;
}


bool Interpreter::is_truthy(const LoxObject& obj) const
{
    switch(obj.get_type())
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

LoxObject Interpreter::evaluate(const std::unique_ptr<Expr<ExprType, VisitType>>& expr)
{
    std::cout << "[" << __func__ << "] evaluating <" << typeid(expr).name() << "> " << expr->to_string() << std::endl;
    return expr->accept(*this);
}

void Interpreter::execute(const std::unique_ptr<Stmt<ExprType, VisitType>>& stmt)
{
    stmt->accept(*this);
}


// ======== EXPRESSION VISITOR FUNCTIONS ======== //
LoxObject Interpreter::visit(LiteralExpr<ExprType, VisitType>& expr)
{
    return expr.value;
}

LoxObject Interpreter::visit(GroupingExpr<ExprType, VisitType>& expr)
{
    return this->evaluate(expr.left);
}

LoxObject Interpreter::visit(UnaryExpr<ExprType, VisitType>& expr)
{
    LoxObject right = this->evaluate(expr.right);

    switch(expr.op.type)
    {
        case TokenType::MINUS:
            return LoxObject(-right.get_double_val());
        case TokenType::BANG:
            return !this->is_truthy(right);
        default:
            break;          // shut linter up
    }

    throw RuntimeError(expr.op, "Invalid operator for Unary Expression");
    return LoxObject();     // unreachable - TODO: should we throw an error here?
}

LoxObject Interpreter::visit(BinaryExpr<ExprType, VisitType>& expr)
{
    LoxObject left = this->evaluate(expr.left);
    LoxObject right = this->evaluate(expr.right);

    switch(expr.op.type)
    {
        // Arithmetic operators
        case TokenType::MINUS:
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_double_val() - right.get_double_val());

        case TokenType::SLASH:
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_double_val() / right.get_double_val());

        case TokenType::STAR:
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_double_val() * right.get_double_val());

        // Lox allows string concat with '+' operator
        case TokenType::PLUS:
            this->check_number_operands(expr.op, left, right);
            if(left.has_string_type() && right.has_string_type())
                return LoxObject(left.get_string_val() + right.get_string_val());
            if(left.has_number_type() && right.has_number_type())
                return LoxObject(left.get_double_val() + right.get_double_val());

            throw RuntimeError(expr.op, "Operands must be two numbers or two strings");
            break;

        // Comparison operators - these only support number types
        case TokenType::GREATER:
            this->check_number_operands(expr.op, left, right);
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_double_val() > right.get_double_val());

        case TokenType::GREATER_EQUAL:
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_double_val() >= right.get_double_val());

        case TokenType::LESS:
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_double_val() >= right.get_double_val());

        case TokenType::LESS_EQUAL:
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_double_val() <= right.get_double_val());

        default:
            break;      // shut linter up
    }

    return LoxObject();     // TODO: unreachable, throw error?
}


LoxObject Interpreter::visit(VariableExpr<ExprType, VisitType>& expr)
{
    return LoxObject();     // TODO: need to define environment
}


// ======== STATEMENT VISITOR FUNCTIONS ======== //
LoxObject Interpreter::visit(PrintStmt<ExprType, StmtVisitType>& stmt)
{
    LoxObject value = this->evaluate(stmt.expr);
    std::cout << value.to_string() << std::endl;

    // NOTE: one downside of this architecture is that I now have to return this 
    // bogus value.
    return LoxObject();
}

LoxObject Interpreter::visit(ExpressionStmt<ExprType, StmtVisitType>& stmt)
{
    // TODO: don't print here (since this isn't a print statement)
    LoxObject value =  this->evaluate(stmt.expr);
    std::cout << value.to_string() << std::endl;
    
    //std::cout << this->evaluate(stmt.expr).to_string() << std::endl;
    return value;
}

LoxObject Interpreter::visit(VariableStmt<ExprType, StmtVisitType>& stmt)
{
    LoxObject value;
    if(stmt.get_expr())
        value = this->evaluate(stmt.expr);

    return value;       // bogus return...
}

// ======== PUBLIC FUNCTIONS ======== //
void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt<ExprType, StmtVisitType>>>& statements)
{
    try {
        for(unsigned i = 0; i < statements.size(); ++i)
            this->execute(statements[i]);
    }
    catch(RuntimeError& e) {
        runtime_error(e);
    }
}


//std::string Interpreter::interpret(const std::unique_ptr<Expr<ExprType, VisitType>>& expr)
//{
//    try {
//        LoxObject value = this->evaluate(expr);
//        return value.to_string();
//    }
//    catch(RuntimeError& e) {
//        runtime_error(e);
//    }
//
//    return "FAILED";  // unreachable, but shuts linter up
//}
