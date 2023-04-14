/*
 * ASTPRINTER
 * Print any Lox AST
 */

#ifndef __ASTPRINTER_HPP
#define __ASTPRINTER_HPP


#include "Expr.hpp"
#include "Statement.hpp"


/*
 * ASTPrinter
 */
struct ASTPrinter : public ExprVisitor<LoxObject, LoxObject>, public StmtVisitor<LoxObject, LoxObject>
{
    using E = LoxObject;
    using T = LoxObject;
    using ExprPtr = std::unique_ptr<Expr<E, T>>;

    public:
        std::string print(Stmt<E, T>& stmt) {
            return stmt.accept(*this).to_string();
        }

        std::string print(Expr<E, T>& expr) {
            return expr.accept(*this).to_string();
        }

        // Expressions
        LoxObject visit(LiteralExpr<E, T>& expr) final
        {
            return expr.value.to_string();
        }

        LoxObject visit(UnaryExpr<E, T>& expr) final
        {
            std::ostringstream oss;
            oss << "(" << expr.op.lexeme << " ";
            oss << expr.right->accept(*this).to_string();
            oss << ")";
            return LoxObject(oss.str());
            //return this->parenthesize(expr.op.lexeme, exprs);
        }

        LoxObject visit(BinaryExpr<E, T>& expr) final
        {
            std::ostringstream oss;
            oss << "(" << expr.op.lexeme;
            oss << " " << expr.left->accept(*this).to_string();
            oss << " " << expr.right->accept(*this).to_string();
            oss << ")";
            return LoxObject(oss.str());
        }

        LoxObject visit(GroupingExpr<E, T>& expr) final
        {
            std::ostringstream oss;
            oss << "(" << expr.left->accept(*this).to_string() << ")";
            return LoxObject(oss.str());
        }

        LoxObject visit(VariableExpr<E, T>& expr) final
        {
            std::ostringstream oss;
            oss << "(var " << expr.token.to_string() << ")";
            //oss << "(var " << expr.left->accept(*this) << ")";
            return LoxObject(oss.str());
        }

        LoxObject visit(AssignmentExpr<E, T>& expr) final 
        {
            std::ostringstream oss;
            oss << expr.token.lexeme << " = " << expr.expr->to_string();
            return LoxObject(oss.str());
        }

        // Statements
        LoxObject visit(PrintStmt<E, T>& stmt) final 
        {
            return stmt.expr->accept(*this);
        }

        LoxObject visit(ExpressionStmt<E, T>& stmt) final
        {
            return stmt.expr->accept(*this);
        }

        LoxObject visit(VariableStmt<E, T>& stmt) final
        {
            return stmt.expr->accept(*this);
        }

        LoxObject visit(BlockStmt<E, T>& stmt) final
        {
            std::ostringstream oss;
            for(unsigned i = 0; i < stmt.statements.size(); ++i)
                oss << stmt.statements[i].get()->accept(*this).to_string() << std::endl;
            return oss.str();
        }
};




#endif /*__ASTPRINTER_HPP*/
