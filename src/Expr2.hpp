/*
 * EXPR2
 * Some alternate implementations of Expr.hpp
 */

#ifndef __EXPR2_HPP
#define __EXPR2_HPP

#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "Object.hpp"


template <typename T> struct BinaryExpr2;
template <typename T> struct GroupingExpr2;
template <typename T> struct LiteralExpr2;
template <typename T> struct UnaryExpr2;


// Visitor object 
template <typename T> struct ExprVisitor2
{
    public:
        virtual T visit(BinaryExpr2<T>& expr) = 0;
        //virtual T visit(GroupingExpr2<T>& expr) = 0;
        virtual T visit(LiteralExpr2<T>& expr) = 0;
        virtual T visit(UnaryExpr2<T>& expr) = 0;
};


template <typename T> struct Expr2
{
    public:
        Expr2() {} 
        virtual ~Expr2() {}

        virtual T           accept(ExprVisitor2<T>& visitor) = 0;
        virtual std::string to_string(void) const = 0;
};


template <typename T> struct LiteralExpr2 : public Expr2<T>
{
    LoxObject value;

    public:
        LiteralExpr2(const Token& tok) : value(LoxObject(tok)) {} 

        bool operator==(const LiteralExpr2<T>& that) const {
            return this->value == that.value;
        }
        
        bool operator!=(const LiteralExpr2<T>& that) const {
            return !(*this == that);
        }
        
        T accept(ExprVisitor2<T>& visitor) final {
            return visitor.visit(*this);
        }

        std::string to_string(void) const final
        {
            if(this->value.has_type())
                return this->value.to_string();
            
            return "nil";
        }
};


template <typename T> struct UnaryExpr2 : public Expr2<T>
{
    std::shared_ptr<Expr2<T>> value;
    Token op;

    public:
        UnaryExpr2(std::shared_ptr<Expr2<T>> v, const Token& t) : value(v), op(t) {}

        bool operator==(const UnaryExpr2<T>& that) const 
        {
            if(this->value.get() != that.value.get())
                return false;
            if(this->op != that.op)
                return false;

            return true;
        }

        bool operator!=(const UnaryExpr2<T>& that) const {
            return !(*this == that);
        }

        T accept(ExprVisitor2<T>& visitor) final {
            return visitor.visit(*this);
        }

        std::string to_string(void) const final
        {
            std::ostringstream oss;

            oss << this->op.to_string() << " " << this->value->to_string();

            return oss.str();
        }
};


template <typename T> struct BinaryExpr2 : public Expr2<T>
{
    std::shared_ptr<Expr2<T>> left;
    std::shared_ptr<Expr2<T>> right;
    Token op;

    public:
        BinaryExpr2(std::shared_ptr<Expr2<T>> l, std::shared_ptr<Expr2<T>> r, const Token& op) : 
            left(l), 
            right(r), 
            op(op) {}

        bool operator==(const BinaryExpr2<T>& that) const 
        {
            if(this->left.get() != that.left.get())
                return false;
            if(this->right.get() != that.right.get())
                return false;
            if(this->op != that.op)
                return false;

            return true;
        }

        bool operator!=(const BinaryExpr2<T>& that) const {
            return !(*this == that);
        }

        T accept(ExprVisitor2<T>& visitor) final {
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


/*
 * ASTPrinter
 */
struct ASTPrinter2 : public ExprVisitor2<std::string>
{
    using ExprPtr = std::shared_ptr<Expr2<std::string>>;

    public:
        std::string print(Expr2<std::string>& expr) {
            return expr.accept(*this);
        }

        std::string parenthesize(const std::string& name, std::vector<ExprPtr>& exprs)
        {
            std::ostringstream oss;

            oss << "(" << name << " ";
            for(const auto& e : exprs)
                oss << e->accept(*this) << " ";
            oss << ")";

            return oss.str();
        }

        std::string visit(LiteralExpr2<std::string>& expr) final
        {
            if(expr.value.has_type())
                return expr.value.to_string();
                //return expr.value.get_val_as_str();
                //return expr.value.get_string_val();

            return "Nil";       // TODO: Think about if there is anything else we might prefer to return
        }

        std::string visit(UnaryExpr2<std::string>& expr) final
        {
            std::vector<ExprPtr> exprs = {expr.value};
            return this->parenthesize(expr.op.lexeme, exprs);
        }

        std::string visit(BinaryExpr2<std::string>& expr) final
        {
            std::vector<ExprPtr> exprs = {expr.left, expr.right};
            return this->parenthesize(expr.op.lexeme, exprs);
        }
};



#endif /*__EXPR2_HPP*/
