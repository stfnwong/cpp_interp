/*
 * STATEMENTS
 *
 */

#ifndef __STATEMENT_HPP
#define __STATEMENT_HPP

#include "Expr.hpp"


enum class StmtType { PRINT, EXPR, VARIABLE, BLOCK, IF, VAR, WHILE };

template <typename E, typename T> struct PrintStmt;
template <typename E, typename T> struct ExpressionStmt;
template <typename E, typename T> struct VariableStmt;
template <typename E, typename T> struct BlockStmt;
template <typename E, typename T> struct IfStmt;
template <typename E, typename T> struct VarStmt;
template <typename E, typename T> struct WhileStmt;


template <typename E, typename T> struct StmtVisitor
{
    public:
        virtual T visit(PrintStmt<E, T>& stmt) = 0;
        virtual T visit(ExpressionStmt<E, T>& stmt) = 0;
        virtual T visit(VariableStmt<E, T>& stmt) = 0;
        virtual T visit(BlockStmt<E, T>& stmt) = 0;
        virtual T visit(IfStmt<E, T>& stmt) = 0;
        virtual T visit(WhileStmt<E, T>& stmt) = 0;
};


/*
 * Stmt
 */
template <typename E, typename T> struct Stmt
{
    public:
        explicit Stmt() = default;
        virtual ~Stmt() = default;
        virtual StmtType get_type(void) const = 0;
        virtual T accept(StmtVisitor<E, T>& visitor) = 0;
        // TODO: not sure about the get_expr() method now... I am using in in
        // unit tests for convenience.
        virtual const Expr<E, T>* get_expr(void) const = 0;
        virtual std::string to_string(void) const = 0;
};



/*
 * PrintStmt
 */
template <typename E, typename T> struct PrintStmt : public Stmt<E, T>
{
    std::unique_ptr<Expr<E, T>> expr;

    public:
        PrintStmt(std::unique_ptr<Expr<E, T>> expr) : expr(std::move(expr)) {}

        StmtType get_type(void) const final {
            return StmtType::PRINT;
        }

        T accept(StmtVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        const Expr<E, T>* get_expr(void) const final {
            return this->expr.get();
        }

        std::string to_string(void) const final
        {
            std::ostringstream oss;
            oss << "PrintStmt<";
            if(this->expr.get())
                oss << this->expr->to_string();
            oss << ">";
            return oss.str();
        }
};


/*
 * ExpressionStmt
 */
template <typename E, typename T> struct ExpressionStmt : public Stmt<E, T>
{
    std::unique_ptr<Expr<E, T>> expr;

    public:
        ExpressionStmt(std::unique_ptr<Expr<E, T>> expr)  : expr(std::move(expr)) {}

        StmtType get_type(void) const final {
            return StmtType::EXPR;
        }

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


/*
 * VariableStmt
 */
template <typename E, typename T> struct VariableStmt : public Stmt<E, T>
{
    Token token;
    std::unique_ptr<Expr<E, T>> expr;

    public:
        VariableStmt(const Token& tok, std::unique_ptr<Expr<E, T>> expr) : 
            token(tok),
            expr(std::move(expr)) {}

        StmtType get_type(void) const final {
            return StmtType::VARIABLE;
        }

        T accept(StmtVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        const Expr<E, T>* get_expr(void) const final {
            return this->expr.get();
        }

        std::string to_string(void) const final
        {
            std::ostringstream oss;
            oss << "VariableStmt<" << this->token.lexeme;
            if(this->expr.get())
                oss << " = " << this->expr->to_string();
            oss << ">";
            return oss.str();
        }
};



/*
 * BlockStmt
 */
template <typename E, typename T> struct BlockStmt : public Stmt<E, T>
{
    std::vector<std::unique_ptr<Stmt<E, T>>> statements;

    public:
        BlockStmt(std::vector<std::unique_ptr<Stmt<E, T>>> s) : 
            statements(std::move(s)) {} 

        StmtType get_type(void) const final {
            return StmtType::BLOCK;
        }

        T accept(StmtVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        const Expr<E, T>* get_expr(void) const final {
            return nullptr;
        }

        std::string to_string(void) const final 
        {
            std::ostringstream oss;
            //oss << "BlockStmt<" << this->statements.size() << ">";
            oss << "BlockStmt<" << std::endl;
            for(unsigned i = 0; i < this->statements.size(); ++i)
                oss << "  " << this->statements[i]->to_string() << "," << std::endl;
            return oss.str();
        }

        unsigned size(void) const {
            return this->statements.size();
        }
};


/*
 * BlockStmt
 */
template <typename E, typename T> struct IfStmt : public Stmt<E, T>
{
    std::unique_ptr<Expr<E, T>> cond;
    std::unique_ptr<Stmt<E, T>> then_branch;
    std::unique_ptr<Stmt<E, T>> else_branch;

    public:
        IfStmt(std::unique_ptr<Expr<E, T>> cond, 
                std::unique_ptr<Stmt<E, T>> t, 
                std::unique_ptr<Stmt<E, T>> e) :
            cond(std::move(cond)),
            then_branch(std::move(t)),
            else_branch(std::move(e)) {}

        StmtType get_type(void) const final {
            return StmtType::IF;
        }

        T accept(StmtVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        const Expr<E, T>* get_expr(void) const final {
            return nullptr;
        }

        std::string to_string(void) const final 
        {
            std::ostringstream oss;
            oss << "IfStmt<" << this->cond.get()->to_string() << " ? "
                << this->then_branch.get()->to_string() << " : " 
                << this->else_branch.get()->to_string()
                << ">";
            return oss.str();
        }

        unsigned size(void) const {
            return this->statements.size();
        }
};




/*
 * WhileStmt
 */
template <typename E, typename T> struct WhileStmt : public Stmt<E, T>
{
    std::unique_ptr<Expr<E, T>> cond;
    std::unique_ptr<Stmt<E, T>> body;
    
    public:
        WhileStmt(std::unique_ptr<Expr<E, T>> cond, std::unique_ptr<Stmt<E, T>> body) :
            cond(std::move(cond)),
            body(std::move(body)) {}

        StmtType get_type(void) const final {
            return StmtType::WHILE;
        }

        T accept(StmtVisitor<E, T>& visitor) final {
            return visitor.visit(*this);
        }

        const Expr<E, T>* get_expr(void) const final {
            return this->cond.get();
        }

        std::string to_string(void) const final 
        {
            std::ostringstream oss;
            oss << "WhileStmt<";
            if(this->cond.get())
                oss << this->cond->to_string();
            if(this->body.get())
                oss << " " << this->body->to_string();
            oss << ">";

            return oss.str();
        }
};


#endif /*__STATEMENT_HPP*/
