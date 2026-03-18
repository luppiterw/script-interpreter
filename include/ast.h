#ifndef AST_H
#define AST_H

#include "token.h"
#include <memory>
#include <vector>
#include <string>
#include <variant>

namespace script_lang {

// Forward declarations
class Node;
class Statement;
class Expression;
class Program;

// Base classes
class Node {
public:
    virtual ~Node() = default;
    virtual std::string tokenLiteral() const = 0;
    virtual std::string toString() const = 0;
};

class Statement : public Node {
public:
    virtual ~Statement() = default;
};

class Expression : public Node {
public:
    virtual ~Expression() = default;
};

// Program node
class Program : public Node {
public:
    std::vector<std::unique_ptr<Statement>> statements;
    
    std::string tokenLiteral() const override {
        if (!statements.empty()) {
            return statements[0]->tokenLiteral();
        }
        return "";
    }
    
    std::string toString() const override {
        std::string result;
        for (const auto& stmt : statements) {
            result += stmt->toString();
        }
        return result;
    }
};

// Identifier expression
class Identifier : public Expression {
public:
    Token token;
    std::string value;
    
    Identifier(const Token& token) : token(token), value(token.literal) {}
    
    std::string tokenLiteral() const override { return value; }
    std::string toString() const override { return value; }
};

// Integer literal
class IntegerLiteral : public Expression {
public:
    Token token;
    int value;
    
    IntegerLiteral(const Token& token) : token(token) {
        value = std::stoi(token.literal);
    }
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override { return token.literal; }
};

// Float literal
class FloatLiteral : public Expression {
public:
    Token token;
    double value;
    
    FloatLiteral(const Token& token) : token(token) {
        value = std::stod(token.literal);
    }
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override { return token.literal; }
};

// String literal
class StringLiteral : public Expression {
public:
    Token token;
    std::string value;
    
    StringLiteral(const Token& token) : token(token), value(token.literal) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override { return "\"" + value + "\""; }
};

// Boolean literal
class BooleanLiteral : public Expression {
public:
    Token token;
    bool value;
    
    BooleanLiteral(const Token& token) : token(token) {
        value = (token.type == TokenType::TRUE);
    }
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override { return token.literal; }
};

// Null literal
class NullLiteral : public Expression {
public:
    Token token;
    
    NullLiteral(const Token& token) : token(token) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override { return "null"; }
};

// Prefix expression (e.g., !x, -x)
class PrefixExpression : public Expression {
public:
    Token token;
    std::string op;
    std::unique_ptr<Expression> right;
    
    PrefixExpression(const Token& token, const std::string& op) 
        : token(token), op(op) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override {
        return "(" + op + right->toString() + ")";
    }
};

// Infix expression (e.g., x + y, a == b)
class InfixExpression : public Expression {
public:
    Token token;
    std::unique_ptr<Expression> left;
    std::string op;
    std::unique_ptr<Expression> right;
    
    InfixExpression(const Token& token, std::unique_ptr<Expression> left, const std::string& op)
        : token(token), left(std::move(left)), op(op) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override {
        return "(" + left->toString() + " " + op + " " + right->toString() + ")";
    }
};

// Let statement
class LetStatement : public Statement {
public:
    Token token;
    std::unique_ptr<Identifier> name;
    std::unique_ptr<Expression> value;
    
    LetStatement(const Token& token) : token(token) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override {
        std::string result = tokenLiteral() + " " + name->toString() + " = ";
        if (value) {
            result += value->toString();
        }
        result += ";";
        return result;
    }
};

// Return statement
class ReturnStatement : public Statement {
public:
    Token token;
    std::unique_ptr<Expression> returnValue;
    
    ReturnStatement(const Token& token) : token(token) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override {
        std::string result = tokenLiteral() + " ";
        if (returnValue) {
            result += returnValue->toString();
        }
        result += ";";
        return result;
    }
};

// Expression statement
class ExpressionStatement : public Statement {
public:
    Token token;
    std::unique_ptr<Expression> expression;
    
    ExpressionStatement(const Token& token) : token(token) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override {
        if (expression) {
            return expression->toString();
        }
        return "";
    }
};

// Block statement
class BlockStatement : public Statement {
public:
    Token token;
    std::vector<std::unique_ptr<Statement>> statements;
    
    BlockStatement(const Token& token) : token(token) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override {
        std::string result = "{ ";
        for (const auto& stmt : statements) {
            result += stmt->toString() + " ";
        }
        result += "}";
        return result;
    }
};

// If expression
class IfExpression : public Expression {
public:
    Token token;
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> consequence;
    std::unique_ptr<BlockStatement> alternative;
    
    IfExpression(const Token& token) : token(token) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override {
        std::string result = "if" + condition->toString() + " " + consequence->toString();
        if (alternative) {
            result += " else " + alternative->toString();
        }
        return result;
    }
};

// Function literal
class FunctionLiteral : public Expression {
public:
    Token token;
    std::vector<std::unique_ptr<Identifier>> parameters;
    std::unique_ptr<BlockStatement> body;
    
    FunctionLiteral(const Token& token) : token(token) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override {
        std::string result = tokenLiteral() + "(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            result += parameters[i]->toString();
            if (i < parameters.size() - 1) {
                result += ", ";
            }
        }
        result += ") " + body->toString();
        return result;
    }
};

// Array literal
class ArrayLiteral : public Expression {
public:
    Token token;
    std::vector<std::unique_ptr<Expression>> elements;
    
    ArrayLiteral(const Token& token) : token(token) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override {
        std::string result = "[";
        for (size_t i = 0; i < elements.size(); ++i) {
            result += elements[i]->toString();
            if (i < elements.size() - 1) {
                result += ", ";
            }
        }
        result += "]";
        return result;
    }
};

// Index expression (e.g., arr[0])
class IndexExpression : public Expression {
public:
    Token token;
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> index;
    
    IndexExpression(const Token& token, std::unique_ptr<Expression> left)
        : token(token), left(std::move(left)) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override {
        return "(" + left->toString() + "[" + index->toString() + "])";
    }
};

// Call expression
class CallExpression : public Expression {
public:
    Token token;
    std::unique_ptr<Expression> function;
    std::vector<std::unique_ptr<Expression>> arguments;
    
    CallExpression(const Token& token, std::unique_ptr<Expression> function)
        : token(token), function(std::move(function)) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override {
        std::string result = function->toString() + "(";
        for (size_t i = 0; i < arguments.size(); ++i) {
            result += arguments[i]->toString();
            if (i < arguments.size() - 1) {
                result += ", ";
            }
        }
        result += ")";
        return result;
    }
};

// While statement
class WhileStatement : public Statement {
public:
    Token token;
    std::unique_ptr<Expression> condition;
    std::unique_ptr<BlockStatement> body;
    
    WhileStatement(const Token& token) : token(token) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override {
        return "while (" + condition->toString() + ") " + body->toString();
    }
};

// Assign expression (e.g., x = 5)
class AssignExpression : public Expression {
public:
    Token token;
    std::unique_ptr<Identifier> name;
    std::unique_ptr<Expression> value;
    
    AssignExpression(const Token& token, std::unique_ptr<Identifier> name)
        : token(token), name(std::move(name)) {}
    
    std::string tokenLiteral() const override { return token.literal; }
    std::string toString() const override {
        return name->toString() + " = " + value->toString();
    }
};

} // namespace script_lang

#endif // AST_H