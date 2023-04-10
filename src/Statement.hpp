/*
 * STATEMENTS
 *
 */

#ifndef __STATEMENT_HPP
#define __STATEMENT_HPP

#include "Expr.hpp"


template <typename E, typename T> struct PrintStmt;
template <typename E, typename T> struct ExpressionStmt;


template <typename E, typename T> struct StmtVisitor
{
    public:
        virtual T visit(PrintStmt<E, T>& stmt) = 0;
        virtual T visit(ExpressionStmt<E, T>& stmt) = 0;
};


// TODO: create a common get_expr() interface
template <typename E, typename T> struct Stmt
{
    public:
        explicit Stmt() = default;
        virtual ~Stmt() = default;
        virtual T accept(StmtVisitor<E, T>& visitor) = 0;
        virtual const Expr<E, T>* get_expr(void) const = 0;
        virtual std::string to_string(void) const = 0;
};


// TODO: make these unique_ptr<Expr<E, T>>
template <typename E, typename T> struct PrintStmt : public Stmt<E, T>
{
    // TODO: why do I have to do double move here?
    std::unique_ptr<Expr<E, T>> expr;

    public:
        PrintStmt(std::unique_ptr<Expr<E, T>> expr) : expr(std::move(expr)) {} 

        T accept(StmtVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        const Expr<E, T>* get_expr(void) const final {
            return this->expr.get();
        }

        std::string to_string(void) const final
        {
            std::ostringstream oss;
            oss << "PrintStmt<" << this->expr->to_string() << ">";
            return oss.str();
        }
};


template <typename E, typename T> struct ExpressionStmt : public Stmt<E, T>
{
    std::unique_ptr<Expr<E, T>> expr;

    public:
        ExpressionStmt(std::unique_ptr<Expr<E, T>> expr)  : expr(std::move(expr)) {} 

        T accept(StmtVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        const Expr<E, T>* get_expr(void) const final {
            return this->expr.get();
        }

        std::string to_string(void) const final 
        {
            std::ostringstream oss;
            oss << "ExpressionStmt<" << this->expr->to_string() << ">";
            return oss.str();
        }
};


// TODO: Statement visitor

#endif /*__STATEMENT_HPP*/
