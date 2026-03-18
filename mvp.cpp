#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

// Simple script interpreter MVP

namespace script_lang {

enum class TokenType {
    INTEGER, IDENTIFIER, PLUS, MINUS, MULTIPLY, DIVIDE, 
    ASSIGN, SEMICOLON, EOF_, ILLEGAL
};

struct Token {
    TokenType type;
    std::string literal;
    Token(TokenType t = TokenType::ILLEGAL, const std::string& lit = "") 
        : type(t), literal(lit) {}
};

class Lexer {
private:
    std::string input;
    size_t position;
    size_t readPosition;
    char ch;

    void readChar() {
        if (readPosition >= input.length()) {
            ch = 0;
        } else {
            ch = input[readPosition];
        }
        position = readPosition;
        readPosition++;
    }

    void skipWhitespace() {
        while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
            readChar();
        }
    }

    std::string readNumber() {
        size_t startPos = position;
        while (ch >= '0' && ch <= '9') {
            readChar();
        }
        return input.substr(startPos, position - startPos);
    }

public:
    explicit Lexer(const std::string& input) 
        : input(input), position(0), readPosition(0), ch(0) {
        readChar();
    }

    Token nextToken() {
        Token token;
        skipWhitespace();

        switch (ch) {
            case '=':
                token = Token(TokenType::ASSIGN, "=");
                break;
            case ';':
                token = Token(TokenType::SEMICOLON, ";");
                break;
            case '+':
                token = Token(TokenType::PLUS, "+");
                break;
            case '-':
                token = Token(TokenType::MINUS, "-");
                break;
            case '*':
                token = Token(TokenType::MULTIPLY, "*");
                break;
            case '/':
                token = Token(TokenType::DIVIDE, "/");
                break;
            case 0:
                token = Token(TokenType::EOF_, "");
                break;
            default:
                if (ch >= '0' && ch <= '9') {
                    token = Token(TokenType::INTEGER, readNumber());
                    return token;
                } else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_') {
                    size_t startPos = position;
                    while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || 
                           (ch >= '0' && ch <= '9') || ch == '_') {
                        readChar();
                    }
                    token = Token(TokenType::IDENTIFIER, input.substr(startPos, position - startPos));
                    return token;
                } else {
                    token = Token(TokenType::ILLEGAL, std::string(1, ch));
                }
                break;
        }

        readChar();
        return token;
    }
};

class Object {
public:
    virtual ~Object() = default;
    virtual std::string inspect() const = 0;
};

using ObjectPtr = std::shared_ptr<Object>;

class Integer : public Object {
public:
    int value;
    explicit Integer(int v) : value(v) {}
    std::string inspect() const override { return std::to_string(value); }
};

ObjectPtr makeInteger(int value) {
    return std::make_shared<Integer>(value);
}

class Evaluator {
private:
    std::unordered_map<std::string, ObjectPtr> environment;

public:
    ObjectPtr evaluate(const std::string& input) {
        Lexer lexer(input);
        Token token = lexer.nextToken();
        
        // Simple expression evaluation: identifier = number
        if (token.type == TokenType::IDENTIFIER) {
            std::string varName = token.literal;
            token = lexer.nextToken();
            
            if (token.type == TokenType::ASSIGN) {
                token = lexer.nextToken();
                if (token.type == TokenType::INTEGER) {
                    int value = std::stoi(token.literal);
                    auto obj = makeInteger(value);
                    environment[varName] = obj;
                    return obj;
                }
            }
        }
        // Simple arithmetic: number operator number
        else if (token.type == TokenType::INTEGER) {
            int left = std::stoi(token.literal);
            token = lexer.nextToken();
            
            if (token.type == TokenType::PLUS) {
                token = lexer.nextToken();
                if (token.type == TokenType::INTEGER) {
                    int right = std::stoi(token.literal);
                    return makeInteger(left + right);
                }
            }
        }
        
        return makeInteger(0);
    }
};

} // namespace script_lang

int main() {
    using namespace script_lang;
    
    std::cout << "Simple Script Interpreter MVP\n";
    std::cout << "Enter expressions like: x = 5 or 3 + 4\n";
    std::cout << "Type 'quit' to exit\n\n";
    
    Evaluator evaluator;
    std::string input;
    
    while (true) {
        std::cout << ">>> ";
        std::getline(std::cin, input);
        
        if (input == "quit") {
            break;
        }
        
        if (!input.empty()) {
            auto result = evaluator.evaluate(input);
            std::cout << result->inspect() << std::endl;
        }
    }
    
    return 0;
}