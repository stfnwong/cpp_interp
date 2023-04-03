/*
 * EXPRESSIONS
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"


#include "Expr.hpp"
#include "Token.hpp"


struct TestVisitor : public ExprVisitor
{
    public:
        LoxObject visit(BinaryExpr& expr) override {
            return LoxObject();
        }
        LoxObject visit(GroupingExpr& expr) override {
            return LoxObject();
        }
        LoxObject visit(LiteralExpr& expr) override {
            return LoxObject();
        }
        LoxObject visit(UnaryExpr& expr) override {
            return LoxObject();
        }
};



TEST_CASE("test_create_binary_expr", "expression")
{
    //LiteralExpr left(
    //        LoxObject(
    //            Token(TokenType::NUMBER, "1", 1, 1.0)
    //        )
    //    );

    //LiteralExpr right(
    //        LoxObject(
    //            Token(TokenType::NUMBER, "1", 1, 1.0)
    //        )
    //    );


    std::shared_ptr<LiteralExpr> left = std::make_shared<LiteralExpr>(
            LoxObject(
                Token(TokenType::NUMBER, "1", 1, 1.0)
            )
        );

    std::shared_ptr<LiteralExpr>  right = std::make_shared<LiteralExpr>(
            LoxObject(
                Token(TokenType::NUMBER, "1", 1, 1.0)
            )
        );


    Token op = Token(TokenType::PLUS, "+", 1);
    BinaryExpr test_expr = BinaryExpr(left, right, op); 

    REQUIRE(test_expr.op == Token(TokenType::PLUS, "+", 1));
    TestVisitor test_visitor;


    LoxObject ret = test_visitor.visit(test_expr);

    std::cout << "[" << __func__ << "] ret: " << ret.to_string() << std::endl;

}

