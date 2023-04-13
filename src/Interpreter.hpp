/*
 * INTERPRETER
 */

#ifndef __INTERPRETER_HPP
#define __INTERPRETER_HPP

#include <iostream>   // TODO: move all the error stuff to Error.hpp

#include "Common.hpp"
#include "Expr.hpp"
#include "Object.hpp"
#include "Statement.hpp"


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



class Interpreter : public ExprVisitor<ExprType, VisitType>, public StmtVisitor<ExprType, StmtVisitType>
{
    private:
        bool is_truthy(const LoxObject& obj) const;
        bool is_equal(const LoxObject& a, const LoxObject& b) const;
        void check_number_operand(const Token& otor, const LoxObject& orand);
        void check_number_operands(const Token& otor, const LoxObject& o1, const LoxObject& o2);

        LoxObject evaluate(const std::unique_ptr<Expr<ExprType, VisitType>>& expr);
        void      execute(const std::unique_ptr<Stmt<ExprType, VisitType>>& stmt);
        
        // Expressions
        LoxObject visit(LiteralExpr<ExprType, VisitType>& expr) final;
        LoxObject visit(GroupingExpr<ExprType, VisitType>& expr) final;
        LoxObject visit(UnaryExpr<ExprType, VisitType>& expr) final;
        LoxObject visit(BinaryExpr<ExprType, VisitType>& expr) final;
        LoxObject visit(VariableExpr<ExprType, VisitType>& expr) final;


        // Statments 
        LoxObject visit(PrintStmt<ExprType, StmtVisitType>& stmt) final;
        LoxObject visit(ExpressionStmt<ExprType, StmtVisitType>& stmt) final;
        LoxObject visit(VariableStmt<ExprType, StmtVisitType>& stmt) final;


    public:
        Interpreter() {} 
        void interpret(const std::vector<std::unique_ptr<Stmt<ExprType, StmtVisitType>>>& statements);
        //std::string interpret(const std::unique_ptr<Expr<ExprType, VisitType>>& expression);
};



#endif /*__INTERPRETER_HPP*/
