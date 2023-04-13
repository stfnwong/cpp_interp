/*
 * STATEMENTS
 *
 */

#ifndef __STATEMENT_HPP
#define __STATEMENT_HPP

#include "Expr.hpp"


template <typename E, typename T> struct PrintStmt;
template <typename E, typename T> struct ExpressionStmt;
template <typename E, typename T> struct VariableStmt;


template <typename E, typename T> struct StmtVisitor
{
    public:
        virtual T visit(PrintStmt<E, T>& stmt) = 0;
        virtual T visit(ExpressionStmt<E, T>& stmt) = 0;
        virtual T visit(VariableStmt<E, T>& stmt) = 0;
};



template <typename E, typename T> struct Stmt
{
    public:
        explicit Stmt() = default;
        virtual ~Stmt() = default;
        virtual T accept(StmtVisitor<E, T>& visitor) = 0;
        // TODO: not sure about the get_expr() method now... I am using in in
        // unit tests for convenience.
        virtual const Expr<E, T>* get_expr(void) const = 0;
        virtual std::string to_string(void) const = 0;
};


template <typename E, typename T> struct PrintStmt : public Stmt<E, T>
{
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


template <typename E, typename T> struct VariableStmt : public Stmt<E, T>
{
    Token token;
    std::unique_ptr<Expr<E, T>> expr;

    public:
        VariableStmt(const Token& tok, std::unique_ptr<Expr<E, T>> expr) : 
            token(tok),
            expr(std::move(expr))
    {}

        T accept(StmtVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        const Expr<E, T>* get_expr(void) const final {
            return this->expr.get();
        }

        std::string to_string(void) const final
        {
            std::ostringstream oss;
            oss << "VariableStmt<" << this->expr->to_string() << ">";
            return oss.str();
        }
};



#endif /*__STATEMENT_HPP*/
