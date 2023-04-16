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


// This is a hack to get around lack of instanceof in C++. I don't know what the 
// preferred design is here... what I am trying to avoid is the use of dynamic_cast<>
// to figure out what the type of an Expr is.
// TODO: think about an alternative for this
enum class ExprType { LITERAL, UNARY, BINARY, GROUPING, VARIABLE, ASSIGNMENT, LOGICAL };


template <typename E, typename T> struct LiteralExpr;
template <typename E, typename T> struct UnaryExpr;
template <typename E, typename T> struct BinaryExpr;
template <typename E, typename T> struct GroupingExpr;
template <typename E, typename T> struct VariableExpr;
template <typename E, typename T> struct AssignmentExpr;
template <typename E, typename T> struct LogicalExpr;


// Visitor object 
template <typename E, typename T> struct ExprVisitor
{
    public:
        virtual T visit(LiteralExpr<E, T>& expr) = 0;
        virtual T visit(UnaryExpr<E, T>& expr) = 0;
        virtual T visit(BinaryExpr<E, T>& expr) = 0;
        virtual T visit(GroupingExpr<E, T>& expr) = 0;
        virtual T visit(VariableExpr<E, T>& expr) = 0;
        virtual T visit(AssignmentExpr<E, T>& expr) = 0;
        virtual T visit(LogicalExpr<E, T>& expr) = 0;
};


/*
 * Abstract Base Class for Expression Objects
 */
template <typename E, typename T> struct Expr
{
    public:
        explicit Expr() = default;
        virtual ~Expr() = default;

        virtual ExprType    get_type(void) const = 0;
        virtual T           accept(ExprVisitor<E, T>& visitor) = 0;
        virtual std::string to_string(void) const = 0;
};



/*
 * LiteralExpr
 */
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

        ExprType get_type(void) const final {
            return ExprType::LITERAL;
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

        std::string to_repr(void) const
        {
            std::ostringstream oss;
            oss << "LiteralExpr<" << this->value.to_repr() << ">";
            return oss.str();
        }
};



/*
 * UnaryExpr
 */
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

        ExprType get_type(void) const final {
            return ExprType::UNARY;
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



/*
 * BinaryExpr
 */
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

        ExprType get_type(void) const final {
            return ExprType::BINARY;
        }

        T accept(ExprVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        std::string to_string(void) const final
        {
            std::ostringstream oss;

            oss << "BinaryExpr<" 
                << this->left->to_string() << " " 
                << this->op.to_string() << " " 
                << this->right->to_string() 
                << ">";

            return oss.str();
        }

        //std::string to_repr(void) const
        //{
        //    std::ostringstream oss;
        //    oss << "BinaryExpr<" 
        //        << this->left->to_string() << ""
        //        << this->op.to_string() << " "
        //        << this->right->to_string() << ">";

        //    return oss.str();
        //}
};



/*
 * GroupingExpr
 */
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

        ExprType get_type(void) const final {
            return ExprType::GROUPING;
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



/*
 * VariableExpr
 */
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

        ExprType get_type(void) const final {
            return ExprType::VARIABLE;
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
 * AssignmentExpr
 */
template <typename E, typename T> struct AssignmentExpr : Expr<E, T>
{
    Token token;
    std::unique_ptr<Expr<E, T>> expr;

    public:
        AssignmentExpr(const Token& tok,  std::unique_ptr<Expr<E, T>> e) :
            token(tok), 
            expr(std::move(e)) {}

        bool operator==(const AssignmentExpr<E, T>& that) const
        {
            if(this->token != that.token)
                return false;
            return this->expr.get() == that.expr.get();
        }

        bool operator!=(const AssignmentExpr<E, T>& that) const {
            return !(*this == that);
        }

        ExprType get_type(void) const final {
            return ExprType::ASSIGNMENT;
        }

        T accept(ExprVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        std::string to_string(void) const final
        {
            std::ostringstream oss;
            oss << "AssignmentExpr<" << this->token.to_string() 
                << " = " << this->expr.get()->to_string() << ">";
            return oss.str();
        }
};


/*
 * LogicalExpr
 */
template <typename E, typename T> struct LogicalExpr : Expr<E, T>
{
    Token op;
    std::unique_ptr<Expr<E, T>> left;
    std::unique_ptr<Expr<E, T>> right;

    public:
        LogicalExpr(const Token& o, std::unique_ptr<Expr<E, T>> l, std::unique_ptr<Expr<E, T>> r) :
            op(o),
            left(std::move(l)),
            right(std::move(r)) {}

        bool operator==(const LogicalExpr<E, T>& that) const 
        {
            if(this->op != that.op)
                return false;
            if(this->left.get() != that.left.get())
                return false;
            if(this->right.get() != that.right.get())
                return false;

            return true;
        }

        bool operator!=(const LogicalExpr<E, T>& that) const {
            return !(*this == that);
        }

        ExprType get_type(void) const final {
            return ExprType::LOGICAL;
        }

        T accept(ExprVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        std::string to_string(void) const final 
        {
            std::ostringstream oss;
            oss << "LogicalExpr<" 
                << this->left.get()->to_string() << " " 
                << this->op.to_string() << " "
                << this->right.get()->to_string() << ">";

            return oss.str();
        }
};


#endif /*__EXPR_HPP*/
