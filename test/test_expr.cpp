/*
 * EXPRESSIONS
 */


#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"


#include "Expr.hpp"
#include "Token.hpp"


struct TestVisitor : public ExprVisitor<std::string>
{
    public:
        std::string visit(BinaryExpr<std::string>& expr) override {
            return LoxObject().to_string();
        }
        std::string visit(GroupingExpr<std::string>& expr) override {
            return LoxObject().to_string();
        }
        std::string visit(LiteralExpr<std::string>& expr) override {
            return LoxObject().to_string();
        }
        std::string visit(UnaryExpr<std::string>& expr) override {
            return LoxObject().to_string();
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


    using T = std::string;
    std::shared_ptr<LiteralExpr<T>> left = std::make_shared<LiteralExpr<T>>(
            LoxObject(
                Token(TokenType::NUMBER, "1", 1, 1.0)
            )
        );

    std::shared_ptr<LiteralExpr<T>>  right = std::make_shared<LiteralExpr<T>>(
            LoxObject(
                Token(TokenType::NUMBER, "1", 1, 1.0)
            )
        );


    Token op = Token(TokenType::PLUS, "+", 1);
    BinaryExpr<T> test_expr = BinaryExpr<T>(left, right, op); 

    REQUIRE(test_expr.op == Token(TokenType::PLUS, "+", 1));
    TestVisitor test_visitor;


    T ret = test_visitor.visit(test_expr);

    std::cout << "[" << __func__ << "] ret: " << ret << std::endl;

}

