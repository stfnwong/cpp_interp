/*
 * INTERPRETER
 */

#ifndef __INTERPRETER_HPP
#define __INTERPRETER_HPP

#include <iostream>   // TODO: move all the error stuff to Error.hpp

#include "Expr.hpp"
#include "Object.hpp"


struct RuntimeError : public std::exception
{
    Token token;
    std::string msg_;

    public:
        explicit RuntimeError(const Token& tok, const std::string& msg) : 
            token(tok), 
            msg_(msg) {} 

        const char* what() {
            return this->msg_.c_str();
        }
};

static bool had_runtime_error = false;

static void runtime_error(RuntimeError& e)
{
    std::cerr << e.what() << std::endl << "[line " << e.token.line << "]";
    had_runtime_error = true;
}


using E = LoxObject;
using T = LoxObject;


class Interpreter : ExprVisitor<E, T>
{
    private:
        bool is_truthy(const LoxObject& obj) const;
        bool is_equal(const LoxObject& a, const LoxObject& b) const;
        void check_number_operand(const Token& otor, const LoxObject& orand);
        void check_number_operands(const Token& otor, const LoxObject& o1, const LoxObject& o2);

        LoxObject evaluate(const std::unique_ptr<Expr<E, T>>& expr);
        
        // Expressions
        LoxObject visit(LiteralExpr<E, T>& expr);
        LoxObject visit(GroupingExpr<E, T>& expr);
        LoxObject visit(UnaryExpr<E, T>& expr);
        LoxObject visit(BinaryExpr<E, T>& expr);

    public:
        Interpreter() {} 
        std::string interpret(const std::unique_ptr<Expr<E, T>>& expression);
};



#endif /*__INTERPRETER_HPP*/
