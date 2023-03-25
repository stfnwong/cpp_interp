/* TOKEN 
 * Token definitions 
 *
 * Stefan Wong 2018
 */

#ifndef __TOKEN_HPP
#define __TOKEN_HPP


#include <optional>
#include <string>
#include <variant>
#include <unordered_map>


enum class TokenType{
    // Single character tokens 
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, 
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
    // 1-2 character tokens 
    BANG, BANG_EQUAL, 
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    // Literals
    IDENTIFIER, STRING, NUMBER,
    // Keywords 
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,
    // End of file 
    LOX_EOF
};

// Array of strings?
extern std::unordered_map<TokenType, std::string> token_to_str;


// Print a Token literal
struct make_literal_string{
    std::string operator()(const std::string& s) const {
        return s;
    }
    std::string operator()(int x) const
    {
        return std::to_string(x);
    }
};

using literal_type = std::variant<std::string, float>; 

/*
 * A single token
 */
struct Token
{
    TokenType type;
    std::string lexeme;
    unsigned line;
    std::optional<literal_type> literal;
    //void *literal;      // what to do about this...?

    public: 
        Token(TokenType type, unsigned line) : type(type), lexeme(""), line(line) {}
        Token(TokenType type, const std::string& lexeme, unsigned line) : type(type), lexeme(lexeme), line(line) {} 
        Token(TokenType type, const std::string& lexeme, unsigned line, const std::string& literal) : type(type), lexeme(lexeme), line(line), literal{literal} {} 
        Token(TokenType type, const std::string& lexeme, unsigned line, float literal) : type(type), lexeme(lexeme), line(line), literal(literal) {} 

        // TODO: Do I need two constructors, one each for string and float versions of 
        // this->literal?
        //Token(TokenType type, const std::string& lexeme, int line, std::variant<std::string, float>literal) : 
        //    type(type), lexeme(lexeme), line(line), literal(literal) {}
        ~Token() {}

        bool has_literal(void) const;
        std::string to_string(void) const;
};




#endif /*__TOKEN_HPP*/
