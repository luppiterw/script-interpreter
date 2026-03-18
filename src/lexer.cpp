#include "lexer.h"
#include "token.h"
#include <stdexcept>

namespace script_lang {

Lexer::Lexer(const std::string& input) 
    : input(input), position(0), readPosition(0), ch(0), line(1), column(1) {
    readChar();
}

void Lexer::readChar() {
    if (readPosition >= input.length()) {
        ch = 0; // EOF
    } else {
        ch = input[readPosition];
    }
    
    if (ch == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    
    position = readPosition;
    readPosition++;
}

void Lexer::skipWhitespace() {
    while (true) {
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
            readChar();
        } else if (ch == '/' && readPosition < input.length() && input[readPosition] == '/') {
            // Single-line comment, skip until newline
            while (ch != '\n' && ch != 0) {
                readChar();
            }
        } else {
            break;
        }
    }
}

Token Lexer::newToken(TokenType type, const std::string& literal) {
    if (type == TokenType::EOF_) {
        return Token(type, "", line, column);
    }
    return Token(type, literal.empty() ? std::string(1, ch) : literal, line, column);
}

std::string Lexer::readIdentifier() {
    size_t startPos = position;
    while (isLetter(ch) || isDigit(ch)) {
        readChar();
    }
    return input.substr(startPos, position - startPos);
}

std::string Lexer::readNumber() {
    size_t startPos = position;
    bool hasDot = false;
    
    while (isDigit(ch) || ch == '.') {
        if (ch == '.') {
            if (hasDot) break; // Second dot, stop reading
            hasDot = true;
        }
        readChar();
    }
    
    return input.substr(startPos, position - startPos);
}

std::string Lexer::readString() {
    size_t startPos = position + 1; // Skip opening quote
    readChar(); // Skip opening quote
    
    while (ch != '"' && ch != 0) {
        if (ch == '\\') {
            readChar(); // Skip escape character
        }
        readChar();
    }
    
    std::string result = input.substr(startPos, position - startPos);
    if (ch == '"') {
        readChar(); // Skip closing quote
    }
    
    return result;
}

bool Lexer::isLetter(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

Token Lexer::nextToken() {
    Token token;
    
    skipWhitespace();
    
    switch (ch) {
        case '=':
            if (input[readPosition] == '=') {
                readChar();
                token = newToken(TokenType::EQUAL, "==");
            } else {
                token = newToken(TokenType::ASSIGN);
            }
            break;
        case '+':
            token = newToken(TokenType::PLUS);
            break;
        case '-':
            token = newToken(TokenType::MINUS);
            break;
        case '*':
            token = newToken(TokenType::MULTIPLY);
            break;
        case '/':
            token = newToken(TokenType::DIVIDE);
            break;
        case '%':
            token = newToken(TokenType::MODULO);
            break;
        case '!':
            if (input[readPosition] == '=') {
                readChar();
                token = newToken(TokenType::NOT_EQUAL, "!=");
            } else {
                token = newToken(TokenType::NOT);
            }
            break;
        case '<':
            if (input[readPosition] == '=') {
                readChar();
                token = newToken(TokenType::LESS_EQUAL, "<=");
            } else {
                token = newToken(TokenType::LESS);
            }
            break;
        case '>':
            if (input[readPosition] == '=') {
                readChar();
                token = newToken(TokenType::GREATER_EQUAL, ">=");
            } else {
                token = newToken(TokenType::GREATER);
            }
            break;
        case '&':
            if (input[readPosition] == '&') {
                readChar();
                token = newToken(TokenType::AND, "&&");
            } else {
                token = newToken(TokenType::ILLEGAL);
            }
            break;
        case '|':
            if (input[readPosition] == '|') {
                readChar();
                token = newToken(TokenType::OR, "||");
            } else {
                token = newToken(TokenType::ILLEGAL);
            }
            break;
        case '(':
            token = newToken(TokenType::LEFT_PAREN);
            break;
        case ')':
            token = newToken(TokenType::RIGHT_PAREN);
            break;
        case '{':
            token = newToken(TokenType::LEFT_BRACE);
            break;
        case '}':
            token = newToken(TokenType::RIGHT_BRACE);
            break;
        case '[':
            token = newToken(TokenType::LEFT_BRACKET);
            break;
        case ']':
            token = newToken(TokenType::RIGHT_BRACKET);
            break;
        case ',':
            token = newToken(TokenType::COMMA);
            break;
        case ';':
            token = newToken(TokenType::SEMICOLON);
            break;
        case '.':
            token = newToken(TokenType::DOT);
            break;
        case '"':
            token = Token(TokenType::STRING, readString(), line, column);
            return token;
        case 0:
            token = newToken(TokenType::EOF_);
            break;
        default:
            if (isLetter(ch)) {
                std::string ident = readIdentifier();
                TokenType type = lookupIdentifier(ident);
                token = Token(type, ident, line, column);
                return token;
            } else if (isDigit(ch)) {
                std::string num = readNumber();
                if (num.find('.') != std::string::npos) {
                    token = Token(TokenType::FLOAT, num, line, column);
                } else {
                    token = Token(TokenType::INTEGER, num, line, column);
                }
                return token;
            } else {
                token = newToken(TokenType::ILLEGAL);
            }
            break;
    }
    
    readChar();
    return token;
}

} // namespace script_lang