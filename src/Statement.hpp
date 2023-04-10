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
        Stmt() {} 
        virtual ~Stmt() {}
        virtual T                           accept(StmtVisitor<E, T>& visitor) = 0;
        virtual std::shared_ptr<Expr<E, T>> get_expr(void) const = 0;
};


// TODO: make these shared_ptr<Expr<E, T>>
template <typename E, typename T> struct PrintStmt : public Stmt<E, T>
{
    std::shared_ptr<Expr<E, T>> expr;

    public:
        //PrintStmt(std::unique_ptr<Expr<E, T>> expr) : expr(expr) {} 
        PrintStmt(std::unique_ptr<Expr<E, T>> expr) : expr(std::move(expr)) {} 

        T accept(StmtVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        std::shared_ptr<Expr<E, T>> get_expr(void) const final {
            return this->expr;
        }
};


template <typename E, typename T> struct ExpressionStmt : public Stmt<E, T>
{
    std::shared_ptr<Expr<E, T>> expr;

    public:
        //ExpressionStmt(std::unique_ptr<Expr<E, T>> expr)  : expr(expr) {} 
        ExpressionStmt(std::unique_ptr<Expr<E, T>> expr)  : expr(std::move(expr)) {} 

        T accept(StmtVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        std::shared_ptr<Expr<E, T>> get_expr(void) const final {
            return this->expr;
        }
};


#endif /*__STATEMENT_HPP*/
