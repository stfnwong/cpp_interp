/*
  RESOVLER
  Resolve variables
 */


#include "Lox.hpp"
#include "Resolver.hpp"



/*
 * resolve()
 */
void Resolver::resolve(std::unique_ptr<Stmt<EType, VType>> stmt)
{
    stmt->accept(*this);
}


/*
 * resolve()
 */
void Resolver::resolve(std::unique_ptr<Expr<EType, VType>> expr)
{
    expr->accept(*this);
}


/*
 * resolve()
 */
void Resolver::resolve(std::vector<std::unique_ptr<Stmt<EType, VType>>>& stmts)
{
    for(unsigned i = 0; i < stmts.size(); ++i)
        this->resolve(std::move(stmts[i]));
}


/*
 * resolve_local()
 */
void Resolver::resolve_local(VariableExpr<EType, VType> expr, const Token& name)
{
    // TODO: if we want to be able to access the 
    for(unsigned i = this->scopes.size(); i >= 0; i--)
    {
        //if(this->scopes[i].find(name.lexeme) != this->scopes.end())
        auto got = this->scopes[i].find(name.lexeme);
        if(got != this->scopes[i].end())
            this->interp.resolve(std::make_unique<VariableExpr<EType, VType>>(expr), scopes.size() - 1 - i);
            //this->interp.resolve(std::make_unique<Expr<EType, VType>>(expr), scopes.size() - 1 - i);

        //if(this->scopes[i].find(name.lexeme))
        //    this->interp.resolve(std::make_unique<Expr<EType, VType>>(expr), scopes.size() - 1 - i);
    }
}


/*
 * begin_scope()
 */
void Resolver::begin_scope(void)
{
    this->scopes.emplace_back(std::unordered_map<std::string, bool>());
}


/*
 * end_scope()
 */
void Resolver::end_scope(void)
{
    this->scopes.pop_back();
}
        


/*
 * declare()
 */
void Resolver::declare(const Token& name)
{
    if(this->scopes.empty())
        return;

    std::unordered_map<std::string, bool> scope = this->scopes.back();
    scope.insert({name.lexeme, false});
}

/*
 * define()
 */
void Resolver::define(const Token& name)
{
    if(this->scopes.empty())
        return;
    this->scopes.back().insert({name.lexeme, true});
}



VType Resolver::visit(VariableExpr<EType, VType>& expr)
{
    if(this->scopes.empty())
    {
        bool var_on_top = this->scopes.back().find(expr.token.lexeme) != this->scopes.front().end() ;
        if(var_on_top && this->scopes.back().find(expr.token.lexeme)->second == false)
            Lox::error(expr.token, "Can't read local variable in its own initializer");  
    }

    this->resolve_local(expr, expr.token);

    return VType();
}



VType Resolver::visit(BlockStmt<EType, VType>& stmt)
{
    this->begin_scope();
    this->resolve(stmt.statements);
    this->end_scope();

    return LoxObject();     // TODO: annoying
}


VType Resolver::visit(VariableStmt<EType, VType>& stmt)
{
    this->declare(stmt.token);
    if(stmt.expr.get())
        this->resolve(std::move(stmt.expr));
    this->define(stmt.token);

    return LoxObject();
}
