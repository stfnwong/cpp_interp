/*
 * EXPR
 * Some alternate implementations of Expr.hpp
 */

#ifndef __EXPR_HPP
#define __EXPR_HPP

#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "Object.hpp"


template <typename E, typename T> struct BinaryExpr;
template <typename E, typename T> struct GroupingExpr;
template <typename E, typename T> struct LiteralExpr;
template <typename E, typename T> struct UnaryExpr;


// Visitor object 
template <typename E, typename T> struct ExprVisitor
{
    public:
        virtual T visit(LiteralExpr<E, T>& expr) = 0;
        virtual T visit(UnaryExpr<E, T>& expr) = 0;
        virtual T visit(BinaryExpr<E, T>& expr) = 0;
        virtual T visit(GroupingExpr<E, T>& expr) = 0;
};


template <typename E, typename T> struct Expr
{
    public:
        Expr() {} 
        virtual ~Expr() {}

        virtual T           accept(ExprVisitor<E, T>& visitor) = 0;
        virtual std::string to_string(void) const = 0;
};


template <typename E, typename T> struct LiteralExpr : public Expr<E, T>
{
    LoxObject value;

    public:
        LiteralExpr(const Token& tok) : value(LoxObject(tok)) {} 

        bool operator==(const LiteralExpr<E, T>& that) const {
            return this->value == that.value;
        }
        
        bool operator!=(const LiteralExpr<E, T>& that) const {
            return !(*this == that);
        }
        
        T accept(ExprVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        std::string to_string(void) const final
        {
            if(this->value.has_type())
                return this->value.to_string();
            
            return "nil";
        }
};


template <typename E, typename T> struct UnaryExpr : public Expr<E, T>
{
    std::shared_ptr<Expr<E, T>> value;
    Token op;

    public:
        UnaryExpr(std::shared_ptr<Expr<E, T>> v, const Token& t) : value(v), op(t) {}

        bool operator==(const UnaryExpr<E, T>& that) const 
        {
            if(this->value.get() != that.value.get())
                return false;
            if(this->op != that.op)
                return false;

            return true;
        }

        bool operator!=(const UnaryExpr<E, T>& that) const {
            return !(*this == that);
        }

        T accept(ExprVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        std::string to_string(void) const final
        {
            std::ostringstream oss;

            oss << this->op.to_string() << " " << this->value->to_string();

            return oss.str();
        }
};


template <typename E, typename T> struct BinaryExpr : public Expr<E, T>
{
    std::shared_ptr<Expr<E, T>> left;
    std::shared_ptr<Expr<E, T>> right;
    Token op;

    public:
        BinaryExpr(std::shared_ptr<Expr<E, T>> l, std::shared_ptr<Expr<E, T>> r, const Token& op) : 
            left(l), 
            right(r), 
            op(op) {}

        bool operator==(const BinaryExpr<E, T>& that) const 
        {
            if(this->left.get() != that.left.get())
                return false;
            if(this->right.get() != that.right.get())
                return false;
            if(this->op != that.op)
                return false;

            return true;
        }

        bool operator!=(const BinaryExpr<E, T>& that) const {
            return !(*this == that);
        }

        T accept(ExprVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        std::string to_string(void) const final
        {
            std::ostringstream oss;

            oss << this->left->to_string() << " " 
                << this->op.to_string() << " " 
                << this->right->to_string();

            return oss.str();
        }
};


template <typename E, typename T> struct GroupingExpr : public Expr<E, T>
{
    std::shared_ptr<Expr<E, T>> expression;

    public:
        GroupingExpr(std::shared_ptr<Expr<E, T>> e) : expression(e) {}

        bool operator==(const GroupingExpr<E, T>& that) const {
            return this->expression.get() == that.expression.get();
        }

        bool operator!=(const GroupingExpr<E, T>& that) const {
            return !(*this == that);
        }

        T accept(ExprVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        std::string to_string(void) const final 
        {
            std::ostringstream oss;
            oss << "Group(" << this->expression->to_string();
            return oss.str();
        }
};


/*
 * ASTPrinter
 */
struct ASTPrinter : public ExprVisitor<LoxObject, std::string>
{
    using E = LoxObject;
    using T = std::string;
    using ExprPtr = std::shared_ptr<Expr<E, T>>;

    public:
        std::string print(Expr<E, T>& expr) {
            return expr.accept(*this);
        }

        std::string parenthesize(const std::string& name, std::vector<ExprPtr>& exprs)
        {
            std::ostringstream oss;

            oss << "(" << name;
            for(const auto& e : exprs)
                oss << " " << e->accept(*this);
            oss << ")";

            return oss.str();
        }

        std::string visit(LiteralExpr<E, T>& expr) final
        {
            if(expr.value.has_type())
                return expr.value.to_string();
                //return expr.value.get_val_as_str();
                //return expr.value.get_string_val();

            return "Nil";       // TODO: Think about if there is anything else we might prefer to return
        }

        std::string visit(UnaryExpr<E, T>& expr) final
        {
            std::vector<ExprPtr> exprs = {expr.value};
            return this->parenthesize(expr.op.lexeme, exprs);
        }

        std::string visit(BinaryExpr<E, T>& expr) final
        {
            std::vector<ExprPtr> exprs = {expr.left, expr.right};
            return this->parenthesize(expr.op.lexeme, exprs);
        }

        std::string visit(GroupingExpr<E, T>& expr) final
        {
            std::vector<ExprPtr> exprs = {expr.expression};
            return this->parenthesize("Group", exprs);
        }
};



#endif /*__EXPR_HPP*/
