/*
 * INTERPRETER
 */


#include <iostream>

#include "Callable.hpp"
#include "Interpreter.hpp"


static bool had_runtime_error = false;

static void runtime_error(RuntimeError& e)
{
    //std::cerr << e.what() << std::endl << "[line " << e.token.line << "] ";
    std::cerr << "[line " << e.token.line << "] " << e.what() << " " << std::endl;
    had_runtime_error = true;
}




Interpreter::Interpreter()
{
    this->define_globals();
    this->env = std::make_shared<Environment>();
    this->globals = this->env;
}

void Interpreter::define_globals(void)
{
    //this->globals.define(
}

// 
bool Interpreter::is_truthy(const LoxObject& obj) const
{
    switch(obj.type)
    {
        case ObjType::NIL:
            return false;
        case ObjType::BOOLEAN:
            return (obj.token.type == TokenType::TRUE) ? true : false;
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


// ======== EXPRESSION VISITOR FUNCTIONS ======== //
LoxObject Interpreter::visit(LiteralExpr<EType, VType>& expr)
{
    return expr.value;
}

LoxObject Interpreter::visit(GroupingExpr<EType, VType>& expr)
{
    return this->evaluate(expr.left);
}

LoxObject Interpreter::visit(UnaryExpr<EType, VType>& expr)
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

LoxObject Interpreter::visit(BinaryExpr<EType, VType>& expr)
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
            return LoxObject(left.get_double_val() < right.get_double_val());

        case TokenType::LESS_EQUAL:
            this->check_number_operands(expr.op, left, right);
            return LoxObject(left.get_double_val() <= right.get_double_val());

        default:
            break;      // shut linter up
    }

    return LoxObject();     // TODO: unreachable, throw error?
}


LoxObject Interpreter::visit(VariableExpr<EType, VType>& expr)
{
    return this->env->get(expr.token);
}

LoxObject Interpreter::visit(AssignmentExpr<EType, VType>& expr)
{
    LoxObject value = this->evaluate(expr.expr);
    this->env->assign(expr.token, value);
    return value;
}

LoxObject Interpreter::visit(LogicalExpr<EType, VType>& expr)
{
    LoxObject left = this->evaluate(expr.left);

    if(expr.op.type == TokenType::OR)
    {
        if(this->is_truthy(left))
            return left;
    }
    else // we only have OR and AND, so this is the AND branch
    {
        if(!this->is_truthy(left))
            return left;
    }

    return this->evaluate(expr.right);
}

LoxObject Interpreter::visit(CallExpr<EType, VType>& expr)
{
    LoxObject callee = this->evaluate(expr.callee);
    if(!callee.has_callable())
    {
        throw RuntimeError(expr.paren, "Object of type " + callee.get_type_string() + 
                " cannot be called."
        );
    }

    std::vector<LoxObject> args;
    for(unsigned i = 0; i < expr.arguments.size(); ++i)
        args.push_back(std::move(this->evaluate(expr.arguments[i])));

    Callable* func = callee.get_callable().get();

    if(args.size() != func->arity())
    {
        throw RuntimeError(expr.paren, "Expected " + std::to_string(func->arity())
                + " arguments, got " + std::to_string(args.size()) + "."
        );
    }

    return func->call(*this, args);
}


// ======== STATEMENT VISITOR FUNCTIONS ======== //
LoxObject Interpreter::visit(PrintStmt<EType, StmtVType>& stmt)
{
    LoxObject value = this->evaluate(stmt.expr);
    std::cout << value.to_string() << std::endl;  
    return value;
}

LoxObject Interpreter::visit(ExpressionStmt<EType, StmtVType>& stmt)
{
    LoxObject value =  this->evaluate(stmt.expr);
    return value;
}

LoxObject Interpreter::visit(VariableStmt<EType, StmtVType>& stmt)
{
    LoxObject value;
    if(stmt.get_expr())
        value = this->evaluate(stmt.expr);
    this->env->define(stmt.token.lexeme, value);

    return value;
}

LoxObject Interpreter::visit(BlockStmt<EType, StmtVType>& stmt)
{
    // TODO: Revisit using shared_ptr here....
    //auto block_env = Environment(std::make_shared<Environment>(this->env));
    //auto block_env = Environment(this->env);
    auto block_env = std::make_shared<Environment>(this->env);
    this->execute_block(stmt.statements, block_env);
    return LoxObject();
}

LoxObject Interpreter::visit(IfStmt<EType, StmtVType>& stmt)
{
    if(this->is_truthy(this->evaluate(stmt.cond)))
        this->execute(stmt.then_branch);
    else if(stmt.else_branch.get())
        this->execute(stmt.else_branch);

    return LoxObject();     // TODO: what to do about bogus objects? Return a LoxObject pointer set to nullptr?
}

LoxObject Interpreter::visit(WhileStmt<EType, StmtVType>& stmt)
{
    while(this->is_truthy(this->evaluate(stmt.cond)))
        this->execute(stmt.body);

    return LoxObject();
}

LoxObject Interpreter::visit(FunctionStmt<EType, StmtVType>& stmt)
{
    LoxFunction f(&stmt);
    LoxObject fobj(std::make_shared<LoxFunction>(f));
    this->env->define(stmt.name.lexeme, std::move(fobj));

    //std::cout << "[" << __func__ << "] env now contains: ";
    //for(const auto& v : this->env.get_vars())
    //    std::cout << v << " ";
    //std::cout << std::endl;
    //std::cout << "[" << __func__ << "] env.get(" << stmt.name.lexeme << "): " <<
    //    env.get(stmt.name).to_string() << std::endl;

    return LoxObject();
}


LoxObject Interpreter::visit(ReturnStmt<EType, StmtVType>& stmt) 
{
    LoxObject value;
    if(stmt.value.get())
        value = this->evaluate(stmt.value);

    throw Return(value);
}


// ======== PUBLIC FUNCTIONS ======== //
void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt<EType, StmtVType>>>& statements)
{
    try {
        for(unsigned i = 0; i < statements.size(); ++i)
            this->execute(statements[i]);
    }
    catch(RuntimeError& e) {
        runtime_error(e);
    }
}


std::shared_ptr<Environment> Interpreter::get_globals(void) const
{
    return this->env;           // TODO; remove this
    //return this->globals;
}

LoxObject Interpreter::evaluate(const std::unique_ptr<Expr<EType, VType>>& expr)
{
    return expr->accept(*this);
}

LoxObject Interpreter::execute(const std::unique_ptr<Stmt<EType, VType>>& stmt)
{
    return stmt->accept(*this);
}

void Interpreter::execute_block(const std::vector<std::unique_ptr<Stmt<EType, VType>>>& stmts, std::shared_ptr<Environment> block_env)
{
    auto prev_env = this->env;

    try 
    {
        this->env = block_env;   // TODO: what is happening here?
        std::cout << "[" << __func__ << "] this->env: " << std::endl;
        std::cout << this->env->to_repr() << std::endl;
        for(unsigned i = 0; i < stmts.size(); ++i)
        {
            //std::cout << "[" << __func__ << "] cur statement: " << stmts[i]->to_string() << std::endl;
            this->execute(stmts[i]);
        }
    }
    catch(RuntimeError& e)
    {
        runtime_error(e);
    }

    this->env = prev_env;

    std::cout << "[" << __func__ << "] reverting, this->env is now :" << std::endl;
    std::cout << this->env->to_repr() << std::endl;
}

