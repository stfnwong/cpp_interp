/*
 * INTERPRETER
 */

#ifndef __INTERPRETER_HPP
#define __INTERPRETER_HPP

#include <iostream>   // TODO: move all the error stuff to Error.hpp

#include "Common.hpp"
#include "Environment.hpp"
#include "Error.hpp"
#include "Expr.hpp"
#include "Object.hpp"
#include "Statement.hpp"



class Interpreter : public ExprVisitor<EType, VType>, public StmtVisitor<EType, StmtVType>
{
    private:
        Environment env;

        bool is_truthy(const LoxObject& obj) const;
        bool is_equal(const LoxObject& a, const LoxObject& b) const;
        void check_number_operand(const Token& otor, const LoxObject& orand);
        void check_number_operands(const Token& otor, const LoxObject& o1, const LoxObject& o2);

        LoxObject evaluate(const std::unique_ptr<Expr<EType, VType>>& expr);
        void      execute(const std::unique_ptr<Stmt<EType, VType>>& stmt);
        void      execute_block(const std::vector<std::unique_ptr<Stmt<EType, VType>>>& stmts, const Environment& env);
        
        // Expressions
        LoxObject visit(LiteralExpr<EType, VType>& expr) final;
        LoxObject visit(GroupingExpr<EType, VType>& expr) final;
        LoxObject visit(UnaryExpr<EType, VType>& expr) final;
        LoxObject visit(BinaryExpr<EType, VType>& expr) final;
        LoxObject visit(VariableExpr<EType, VType>& expr) final;
        LoxObject visit(AssignmentExpr<EType, VType>& expr) final;


        // Statments 
        LoxObject visit(PrintStmt<EType, StmtVType>& stmt) final;
        LoxObject visit(ExpressionStmt<EType, StmtVType>& stmt) final;
        LoxObject visit(VariableStmt<EType, StmtVType>& stmt) final;
        LoxObject visit(BlockStmt<EType, StmtVType>& stmt) final;
        LoxObject visit(IfStmt<EType, StmtVType>& stmt) final;

    public:
        Interpreter() {} 
        void interpret(const std::vector<std::unique_ptr<Stmt<EType, StmtVType>>>& statements);
};



#endif /*__INTERPRETER_HPP*/
