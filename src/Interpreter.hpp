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



/*
 * Interpret a series of valid Lox statements
 */
class Interpreter : public ExprVisitor<EType, VType>, public StmtVisitor<EType, StmtVType>
{
    private:
        Environment globals;
        Environment env;            // current environment (pointer)?

        void      define_globals(void);
        bool      is_truthy(const LoxObject& obj) const;
        bool      is_equal(const LoxObject& a, const LoxObject& b) const;
        void      check_number_operand(const Token& otor, const LoxObject& orand);
        void      check_number_operands(const Token& otor, const LoxObject& o1, const LoxObject& o2);
        
        // TODO: since this is all internal these could all return LoxObject*, which 
        // could be set to nullptr (rather than constructing a null LoxObject as is 
        // the case now).
        
        // Expressions
        LoxObject visit(LiteralExpr<EType, VType>& expr) final;
        LoxObject visit(GroupingExpr<EType, VType>& expr) final;
        LoxObject visit(UnaryExpr<EType, VType>& expr) final;
        LoxObject visit(BinaryExpr<EType, VType>& expr) final;
        LoxObject visit(VariableExpr<EType, VType>& expr) final;
        LoxObject visit(AssignmentExpr<EType, VType>& expr) final;
        LoxObject visit(LogicalExpr<EType, VType>& expr) final;
        LoxObject visit(CallExpr<EType, VType>& expr) final;

        // Statments 
        LoxObject visit(PrintStmt<EType, StmtVType>& stmt) final;
        LoxObject visit(ExpressionStmt<EType, StmtVType>& stmt) final;
        LoxObject visit(VariableStmt<EType, StmtVType>& stmt) final;
        LoxObject visit(BlockStmt<EType, StmtVType>& stmt) final;
        LoxObject visit(IfStmt<EType, StmtVType>& stmt) final;
        LoxObject visit(WhileStmt<EType, StmtVType>& stmt) final;
        LoxObject visit(FunctionStmt<EType, StmtVType>& stmt) final;
        LoxObject visit(ReturnStmt<EType, StmtVType>& stmt) final;

    public:
        Interpreter();
        void interpret(const std::vector<std::unique_ptr<Stmt<EType, StmtVType>>>& statements);
        Environment get_globals(void) const;

        LoxObject evaluate(const std::unique_ptr<Expr<EType, VType>>& expr);
        void      execute(const std::unique_ptr<Stmt<EType, VType>>& stmt);
        void      execute_block(const std::vector<std::unique_ptr<Stmt<EType, VType>>>& stmts, const Environment& env);
        //void      execute_block(const std::vector<std::unique_ptr<Stmt<EType, VType>>>& stmts, const Environment& env);
};



#endif /*__INTERPRETER_HPP*/
