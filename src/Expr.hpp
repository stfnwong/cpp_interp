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


template <typename T> struct BinaryExpr;
template <typename T> struct GroupingExpr;
template <typename T> struct LiteralExpr;
template <typename T> struct UnaryExpr;


// Visitor object 
template <typename T> struct ExprVisitor
{
    public:
        virtual T visit(LiteralExpr<T>& expr) = 0;
        virtual T visit(UnaryExpr<T>& expr) = 0;
        virtual T visit(BinaryExpr<T>& expr) = 0;
        virtual T visit(GroupingExpr<T>& expr) = 0;
};


template <typename T> struct Expr
{
    public:
        Expr() {} 
        virtual ~Expr() {}

        virtual T           accept(ExprVisitor<T>& visitor) = 0;
        virtual std::string to_string(void) const = 0;
};


template <typename T> struct LiteralExpr : public Expr<T>
{
    LoxObject value;

    public:
        LiteralExpr(const Token& tok) : value(LoxObject(tok)) {} 

        bool operator==(const LiteralExpr<T>& that) const {
            return this->value == that.value;
        }
        
        bool operator!=(const LiteralExpr<T>& that) const {
            return !(*this == that);
        }
        
        T accept(ExprVisitor<T>& visitor) final {
            return visitor.visit(*this);
        }

        std::string to_string(void) const final
        {
            if(this->value.has_type())
                return this->value.to_string();
            
            return "nil";
        }
};


template <typename T> struct UnaryExpr : public Expr<T>
{
    std::shared_ptr<Expr<T>> value;
    Token op;

    public:
        UnaryExpr(std::shared_ptr<Expr<T>> v, const Token& t) : value(v), op(t) {}

        bool operator==(const UnaryExpr<T>& that) const 
        {
            if(this->value.get() != that.value.get())
                return false;
            if(this->op != that.op)
                return false;

            return true;
        }

        bool operator!=(const UnaryExpr<T>& that) const {
            return !(*this == that);
        }

        T accept(ExprVisitor<T>& visitor) final {
            return visitor.visit(*this);
        }

        std::string to_string(void) const final
        {
            std::ostringstream oss;

            oss << this->op.to_string() << " " << this->value->to_string();

            return oss.str();
        }
};


template <typename T> struct BinaryExpr : public Expr<T>
{
    std::shared_ptr<Expr<T>> left;
    std::shared_ptr<Expr<T>> right;
    Token op;

    public:
        BinaryExpr(std::shared_ptr<Expr<T>> l, std::shared_ptr<Expr<T>> r, const Token& op) : 
            left(l), 
            right(r), 
            op(op) {}

        bool operator==(const BinaryExpr<T>& that) const 
        {
            if(this->left.get() != that.left.get())
                return false;
            if(this->right.get() != that.right.get())
                return false;
            if(this->op != that.op)
                return false;

            return true;
        }

        bool operator!=(const BinaryExpr<T>& that) const {
            return !(*this == that);
        }

        T accept(ExprVisitor<T>& visitor) final {
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


template <typename T> struct GroupingExpr : public Expr<T>
{
    std::shared_ptr<Expr<T>> expression;

    public:
        GroupingExpr(std::shared_ptr<Expr<T>> e) : expression(e) {}

        bool operator==(const GroupingExpr<T>& that) const {
            return this->expression.get() == that.expression.get();
        }

        bool operator!=(const GroupingExpr<T>& that) const {
            return !(*this == that);
        }

        T accept(ExprVisitor<T>& visitor) final {
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
struct ASTPrinter2 : public ExprVisitor<std::string>
{
    using T = std::string;
    using ExprPtr = std::shared_ptr<Expr<T>>;

    public:
        std::string print(Expr<std::string>& expr) {
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

        std::string visit(LiteralExpr<std::string>& expr) final
        {
            if(expr.value.has_type())
                return expr.value.to_string();
                //return expr.value.get_val_as_str();
                //return expr.value.get_string_val();

            return "Nil";       // TODO: Think about if there is anything else we might prefer to return
        }

        std::string visit(UnaryExpr<std::string>& expr) final
        {
            std::vector<ExprPtr> exprs = {expr.value};
            return this->parenthesize(expr.op.lexeme, exprs);
        }

        std::string visit(BinaryExpr<std::string>& expr) final
        {
            std::vector<ExprPtr> exprs = {expr.left, expr.right};
            return this->parenthesize(expr.op.lexeme, exprs);
        }

        std::string visit(GroupingExpr<T>& expr) final
        {
            std::vector<ExprPtr> exprs = {expr.expression};
            return this->parenthesize("Group", exprs);
        }
};



#endif /*__EXPR_HPP*/
