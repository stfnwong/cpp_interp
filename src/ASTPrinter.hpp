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
struct ASTPrinter : public ExprVisitor<LoxObject, std::string>, public StmtVisitor<LoxObject, std::string>
{
    using E = LoxObject;
    using T = std::string;
    using ExprPtr = std::unique_ptr<Expr<E, T>>;

    public:
        std::string print(Stmt<E, T>& stmt) {
            return stmt.accept(*this);
        }

        std::string print(Expr<E, T>& expr) {
            return expr.accept(*this);
        }

        // Expressions
        std::string visit(LiteralExpr<E, T>& expr) final
        {
            return expr.value.to_string();
        }

        std::string visit(UnaryExpr<E, T>& expr) final
        {
            std::ostringstream oss;
            oss << "(" << expr.op.lexeme << " ";
            oss << expr.right->accept(*this);
            oss << ")";
            return oss.str();
            //return this->parenthesize(expr.op.lexeme, exprs);
        }

        std::string visit(BinaryExpr<E, T>& expr) final
        {
            std::ostringstream oss;
            oss << "(" << expr.op.lexeme;
            oss << " " << expr.left->accept(*this);
            oss << " " << expr.right->accept(*this);
            oss << ")";
            return oss.str();
        }

        std::string visit(GroupingExpr<E, T>& expr) final
        {
            std::ostringstream oss;
            oss << "(" << expr.left->accept(*this) << ")";
            return oss.str();
        }

        std::string visit(VariableExpr<E, T>& expr) final
        {
            std::ostringstream oss;
            oss << "(var " << expr.token.to_string() << ")";
            //oss << "(var " << expr.left->accept(*this) << ")";
            return oss.str();
        }

        std::string visit(AssignmentExpr<E, T>& expr) final 
        {
            std::ostringstream oss;
            oss << expr.token.lexeme << " = " << expr.expr->to_string();
            return oss.str();
        }

        // Statements
        std::string visit(PrintStmt<E, T>& stmt) final 
        {
            return stmt.expr->accept(*this);
        }

        std::string visit(ExpressionStmt<E, T>& stmt) final
        {
            return stmt.expr->accept(*this);
        }

        std::string visit(VariableStmt<E, T>& stmt) final
        {
            return stmt.expr->accept(*this);
        }

        std::string visit(BlockStmt<E, T>& stmt) final
        {
            std::ostringstream oss;
            for(unsigned i = 0; i < stmt.statements.size(); ++i)
                oss << stmt.statements[i].get()->accept(*this);
            return oss.str();
        }
};




#endif /*__ASTPRINTER_HPP*/
