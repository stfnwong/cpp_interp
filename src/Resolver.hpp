/*
  RESOVLER
  Resolve variables
 */

#ifndef __RESOLVER_HPP
#define __RESOLVER_HPP


#include <unordered_map>
#include <vector>

#include "Common.hpp"
#include "Expr.hpp"
#include "Statement.hpp"
#include "Interpreter.hpp"




class Resolver : public ExprVisitor<EType, VType>, public StmtVisitor<EType, VType>
{
    Interpreter interp;   // TODO: move this out?
    std::vector<std::unordered_map<std::string, bool>> scopes;

    private:
        void resolve(std::unique_ptr<Stmt<EType, VType>> stmt);
        void resolve(std::unique_ptr<Expr<EType, VType>> expr);
        void resolve(std::vector<std::unique_ptr<Stmt<EType, VType>>>& stmts);
        void resolve_local(VariableExpr<EType, VType> expr, const Token& name);
        void begin_scope(void);
        void end_scope(void);


        void declare(const Token& name);
        void define(const Token& name);

    public:
        Resolver(Interpreter& i) : interp(i) {} 

        VType visit(VariableExpr<EType, VType>& expr) override;

        VType visit(BlockStmt<EType, VType>& stmt) override;
        VType visit(VariableStmt<EType, VType>& stmt) override;


};




#endif /*__RESOLVER_HPP*/
