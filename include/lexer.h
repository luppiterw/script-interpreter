#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <cctype>

namespace script_lang {

class Lexer {
private:
    std::string input;
    size_t position;
    size_t readPosition;
    char ch;
    size_t line;
    size_t column;

    void readChar();
    void skipWhitespace();
    Token newToken(TokenType type, const std::string& literal = "");
    std::string readIdentifier();
    std::string readNumber();
    std::string readString();
    bool isLetter(char c) const;
    bool isDigit(char c) const;

public:
    explicit Lexer(const std::string& input);
    Token nextToken();
    size_t getCurrentLine() const { return line; }
    size_t getCurrentColumn() const { return column; }
};

} // namespace script_lang

#endif // LEXER_H