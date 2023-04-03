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
struct GroupingExpr;
struct LiteralExpr;
struct UnaryExpr;


// Visitor object 
struct ExprVisitor
{
    public:
        virtual LoxObject visit(BinaryExpr& expr) = 0;
        virtual LoxObject visit(GroupingExpr& expr) = 0;
        virtual LoxObject visit(LiteralExpr& expr) = 0;
        virtual LoxObject visit(UnaryExpr& expr) = 0;
};


// Base struct for all Expr types
struct Expr
{
    public:
        Expr() {} 
        virtual ~Expr() {}
        virtual LoxObject accept(ExprVisitor& vistor) = 0;
};


using ExprPtr = std::shared_ptr<Expr>;

struct BinaryExpr : public Expr
{
    ExprPtr left;
    ExprPtr right;
    Token op;

    public:
        BinaryExpr(ExprPtr left, ExprPtr right, const Token& op) : 
            left(left), 
            op(op), 
            right(right) {}

        // Comparison (for unit tests, etc)
        LoxObject accept(ExprVisitor& visitor) {
            return visitor.visit(*this);
        }
};


struct GroupingExpr : public Expr
{
    ExprPtr expr;

    public:
        GroupingExpr(ExprPtr expr) : expr(expr) {} 
        LoxObject accept(ExprVisitor& visitor) override {
            return visitor.visit(*this);
        }
};


struct UnaryExpr : public Expr
{
    ExprPtr expr;
    Token op;

    public:
        UnaryExpr(ExprPtr expr, const Token& op) : 
            expr(expr),
            op(op) {}
        LoxObject accept(ExprVisitor& visitor) final { 
            return visitor.visit(*this);
        }
};


struct LiteralExpr : public Expr
{
    LoxObject literal;

    public:
        LiteralExpr(const LoxObject& obj) : literal(obj) {} 
        LoxObject accept(ExprVisitor& visitor) final {
            return visitor.visit(*this);
        }
};



struct AstPrinter : public ExprVisitor
{
    public:
        AstPrinter() {}

        //std::string print(const Expr& expr) {
        //    return expr.accept(this);
        //}

        std::string parenthesize(const std::string& name, std::vector<ExprPtr>& exprs)
        {
            std::ostringstream oss;

            oss << "(" << name;
            for(const auto& e : exprs)
                oss << e->accept(*this).to_string() << " ";
            oss << ")";

            return oss.str();
        }
        
        std::string visit(const BinaryExpr& expr)
        {
            std::vector<ExprPtr> exprs = {expr.left, expr.right};
            return parenthesize(expr.op.lexeme, exprs);
        }

};

#endif /*__EXPR_HPP*/
