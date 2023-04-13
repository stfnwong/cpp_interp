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
template <typename E, typename T> struct VariableExpr;


// Visitor object 
template <typename E, typename T> struct ExprVisitor
{
    public:
        virtual T visit(LiteralExpr<E, T>& expr) = 0;
        virtual T visit(UnaryExpr<E, T>& expr) = 0;
        virtual T visit(BinaryExpr<E, T>& expr) = 0;
        virtual T visit(GroupingExpr<E, T>& expr) = 0;
        virtual T visit(VariableExpr<E, T>& expr) = 0;
};


/*
 * Abstract Base Class for Expression Objects
 */
template <typename E, typename T> struct Expr
{
    public:
        explicit Expr() = default;
        virtual ~Expr() = default;

        virtual T           accept(ExprVisitor<E, T>& visitor) = 0;
        virtual std::string to_string(void) const = 0;
};


template <typename E, typename T> struct LiteralExpr : public Expr<E, T>
{
    LoxObject value;

    public:
        LiteralExpr(const Token& tok) : value(LoxObject(tok)) {} 
        LiteralExpr(const LoxObject& o) : value(o) {}
        ~LiteralExpr() {} 

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
            std::ostringstream oss;
            oss << "LiteralExpr<" << this->value.to_repr() << ">";
            //oss << "LiteralExpr<" << this->value.to_string() << ">";
            return oss.str();

            //if(this->value.has_type())
            //    return this->value.to_string();
            //
            //return "nil";
        }
};


template <typename E, typename T> struct UnaryExpr : public Expr<E, T>
{
    std::unique_ptr<Expr<E, T>> right;
    Token op;

    public:
        UnaryExpr(std::unique_ptr<Expr<E, T>> v, const Token& t) : 
            right(std::move(v)), 
            op(t) {}

        ~UnaryExpr() {}

        bool operator==(const UnaryExpr<E, T>& that) const 
        {
            if(this->right.get() != that.right.get())
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

            oss << this->op.to_string() << " " << this->right->to_string();

            return oss.str();
        }
};


template <typename E, typename T> struct BinaryExpr : public Expr<E, T>
{
    std::unique_ptr<Expr<E, T>> left;
    std::unique_ptr<Expr<E, T>> right;
    Token op;

    public:
        BinaryExpr(std::unique_ptr<Expr<E, T>> l, std::unique_ptr<Expr<E, T>> r, const Token& op) : 
            left(std::move(l)), 
            right(std::move(r)), 
            op(op) {}

        ~BinaryExpr() {} 

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
    std::unique_ptr<Expr<E, T>> left;

    public:
        GroupingExpr(std::unique_ptr<Expr<E, T>> e) : left(std::move(e)) {}
        ~GroupingExpr() {}

        bool operator==(const GroupingExpr<E, T>& that) const {
            return this->left.get() == that.left.get();
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
            oss << "Group(" << this->left->to_string();
            return oss.str();
        }
};


template <typename E, typename T> struct VariableExpr : public Expr<E, T>
{
    //std::unique_ptr<Expr<E, T>> left;
    Token token;

    public:
        VariableExpr(const Token& tok) : token(tok) {}
        ~VariableExpr() {} 

        bool operator==(const VariableExpr<E, T>& that) const {
            return this->left.get() == that.left.get();
        }

        bool operator!=(const VariableExpr<E, T>& that) const {
            return !(*this == that);
        }

        T accept(ExprVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        std::string to_string(void) const final
        {
            std::ostringstream oss;
            oss << "VariableExpr<" << this->token.to_string() << ">";
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
    using ExprPtr = std::unique_ptr<Expr<E, T>>;

    public:
        std::string print(Expr<E, T>& expr) {
            return expr.accept(*this);
        }

        //std::string parenthesize(const std::string& name, std::vector<ExprPtr>& exprs)
        //{
        //    std::ostringstream oss;

        //    oss << "(" << name;
        //    for(const auto& e : exprs)
        //        oss << " " << e->accept(*this);
        //    oss << ")";

        //    return oss.str();
        //}

        std::string visit(LiteralExpr<E, T>& expr) final
        {
            if(expr.value.has_type())
                return expr.value.to_string();

            return "Nil";       // TODO: Think about if there is anything else we might prefer to return
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
};



#endif /*__EXPR_HPP*/
