#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <variant>
#include <iostream>

namespace script_lang {

enum class TokenType {
    // Literals
    INTEGER,
    FLOAT,
    STRING,
    BOOLEAN,
    IDENTIFIER,
    
    // Keywords
    LET,
    CONST,
    FUNCTION,
    CLASS,
    IF,
    ELSE,
    WHILE,
    FOR,
    RETURN,
    TRUE,
    FALSE,
    NULL_,
    
    // Operators
    PLUS,           // +
    MINUS,          // -
    MULTIPLY,       // *
    DIVIDE,         // /
    MODULO,         // %
    ASSIGN,         // =
    EQUAL,          // ==
    NOT_EQUAL,      // !=
    LESS,           // <
    LESS_EQUAL,     // <=
    GREATER,        // >
    GREATER_EQUAL,  // >=
    AND,            // &&
    OR,             // ||
    NOT,            // !
    
    // Delimiters
    LEFT_PAREN,     // (
    RIGHT_PAREN,    // )
    LEFT_BRACE,     // {
    RIGHT_BRACE,    // }
    LEFT_BRACKET,   // [
    RIGHT_BRACKET,  // ]
    COMMA,          // ,
    SEMICOLON,      // ;
    DOT,            // .
    
    // Special
    EOF_,
    ILLEGAL
};

struct Token {
    TokenType type;
    std::string literal;
    size_t line;
    size_t column;
    
    Token(TokenType t = TokenType::ILLEGAL, const std::string& lit = "", 
          size_t l = 0, size_t c = 0) 
        : type(t), literal(lit), line(l), column(c) {}
    
    bool operator==(const Token& other) const {
        return type == other.type && literal == other.literal;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Token& token) {
        os << "Token{" << static_cast<int>(token.type) << ", \"" << token.literal 
           << "\", line:" << token.line << ", col:" << token.column << "}";
        return os;
    }
};

std::string tokenTypeToString(TokenType type);
TokenType lookupIdentifier(const std::string& ident);

} // namespace script_lang

#endif // TOKEN_H