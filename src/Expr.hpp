/*
 * EXPR
 * Expressions 
 */

#ifndef __EXPR_HPP
#define __EXPR_HPP

#include <memory>
#include <sstream>
#include <vector>

#include "Token.hpp"
#include "Object.hpp"


struct BinaryExpr;
template <typename T> struct GroupingExpr<T>;
struct LiteralExpr;
struct UnaryExpr;


// Visitor object 
template <typename T> struct ExprVisitor
{
    public:
        virtual T visit(BinaryExpr<T>& expr) = 0;
        virtual T visit(GroupingExpr<T>& expr) = 0;
        virtual T visit(LiteralExpr<T>& expr) = 0;
        virtual T visit(UnaryExpr<T>& expr) = 0;
};


// Base struct for all Expr types
template <typename T> struct Expr
{
    public:
        Expr() {} 
        virtual ~Expr() {}
        virtual LoxObject accept(ExprVisitor<T>& vistor) = 0;
};


//using std::shared_ptr<Expr<T>> = std::shared_ptr<Expr::template>;

template <typename T> struct BinaryExpr : public Expr<T>
{
    std::shared_ptr<Expr<T>> left;
    std::shared_ptr<Expr<T>> right;
    Token op;

    public:
        BinaryExpr(std::shared_ptr<Expr<T>> left, std::shared_ptr<Expr<T>> right, const Token& op) : 
            left(left), 
            op(op), 
            right(right) {}

        // Comparison (for unit tests, etc)
        T accept(ExprVisitor<T>& visitor) {
            return visitor.visit(*this);
        }
};


template <typename T> struct GroupingExpr : public Expr<T>
{
    std::shared_ptr<Expr<T>> expr;

    public:
        GroupingExpr(std::shared_ptr<Expr<T>> expr) : expr(expr) {} 
        T accept(ExprVisitor<T>& visitor) override {
            return visitor.visit(*this);
        }
};


template <typename T> struct UnaryExpr : public Expr<T>
{
    std::shared_ptr<Expr<T>> expr;
    Token op;

    public:
        UnaryExpr(std::shared_ptr<Expr<T>> expr, const Token& op) : 
            expr(expr),
            op(op) {}
        T accept(ExprVisitor<T>& visitor) final { 
            return visitor.visit(*this);
        }
};


template <typename T> struct LiteralExpr : public Expr<T>
{
    LoxObject literal;

    public:
        LiteralExpr(const LoxObject& obj) : literal(obj) {} 
        T accept(ExprVisitor<T>& visitor) final {
            return visitor.visit(*this);
        }
};



struct AstPrinter : public ExprVisitor<std::string>
{
    public:
        AstPrinter() {}

        //std::string print(const Expr& expr) {
        //    return expr.accept(this);
        //}

        std::string parenthesize(const std::string& name, std::vector<std::shared_ptr<Expr<std::string>>>& exprs)
        {
            std::ostringstream oss;

            oss << "(" << name;
            for(const auto& e : exprs)
                oss << e->accept(*this).to_string() << " ";
            oss << ")";

            return oss.str();
        }
        
        std::string visit(BinaryExpr& expr) final
        {
            std::vector<std::shared_ptr<Expr<std::string>>> exprs = {expr.left, expr.right};
            return this->parenthesize(expr.op.lexeme, exprs);
        }

        std::string visit(GroupingExpr& expr) final
        {
            return "";
        }

        std::string visit(UnaryExpr& expr) final
        {
            std::vector<std::shared_ptr<Expr<std::string>>> exprs = {expr.expr};
            return this->parenthesize(expr.op.lexeme, exprs);
        }

};

#endif /*__EXPR_HPP*/
