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
    LOX_EOF,
    // Empty token 
    EMPTY
};

// Array of strings?
extern std::unordered_map<TokenType, std::string> token_to_str;


// Print a Token literal
struct make_literal_string{
    std::string operator()(const std::string& s) const {
        return s;
    }
    std::string operator()(int x) const {
        return std::to_string(x);
    }
};


/*
 * A single token
 */
struct Token
{
    using literal_type = std::variant<float, bool, std::string>; 

    TokenType type;
    std::string lexeme;
    unsigned line;
    std::optional<literal_type> literal;
    //void *literal;      // what to do about this...?

    public: 
        Token() : type(TokenType::EMPTY), lexeme(""), line(0) {}
        Token(TokenType type, unsigned line) : type(type), lexeme(""), line(line) {}
        Token(TokenType type, const std::string& lexeme, unsigned line) : 
            type(type), lexeme(lexeme), line(line) {} 
        Token(TokenType type, const std::string& lexeme, unsigned line, const std::string& literal) : 
            type(type), lexeme(lexeme), line(line), literal{literal} {} 
        Token(TokenType type, const std::string& lexeme, unsigned line, float literal) : 
            type(type), lexeme(lexeme), line(line), literal(literal) {} 

        ~Token() {}

        // operators 
        bool operator==(const Token& that) const;
        bool operator!=(const Token& that) const;

        std::string get_string_literal(void) const;
        float get_float_literal(void) const;
        bool has_literal(void) const;
        std::string to_string(void) const;
};




#endif /*__TOKEN_HPP*/
