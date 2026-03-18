#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include <functional>
#include <unordered_map>

namespace script_lang {

enum class Precedence {
    LOWEST,
    EQUALS,      // ==
    LESSGREATER, // > or <
    SUM,         // +
    PRODUCT,     // *
    PREFIX,      // -X or !X
    CALL         // myFunction(X)
};

class Parser {
private:
    Lexer lexer;
    Token curToken;
    Token peekToken;
    
    using prefixParseFn = std::function<std::unique_ptr<Expression>()>;
    using infixParseFn = std::function<std::unique_ptr<Expression>(std::unique_ptr<Expression>)>;
    
    std::unordered_map<TokenType, prefixParseFn> prefixParseFns;
    std::unordered_map<TokenType, infixParseFn> infixParseFns;
    std::unordered_map<TokenType, Precedence> precedences;
    
    void nextToken();
    bool expectPeek(TokenType type);
    bool curTokenIs(TokenType type) const;
    bool peekTokenIs(TokenType type) const;
    
    // Error handling
    std::vector<std::string> errors;
    void peekError(TokenType type);
    
    // Precedence
    Precedence curPrecedence() const;
    Precedence peekPrecedence() const;
    
    // Parsing functions
    std::unique_ptr<Program> parseProgram();
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Expression> parseExpression(Precedence precedence);
    
    // Statement parsing
    std::unique_ptr<LetStatement> parseLetStatement();
    std::unique_ptr<ReturnStatement> parseReturnStatement();
    std::unique_ptr<ExpressionStatement> parseExpressionStatement();
    
    // Expression parsing
    std::unique_ptr<Expression> parseIdentifier();
    std::unique_ptr<Expression> parseIntegerLiteral();
    std::unique_ptr<Expression> parseFloatLiteral();
    std::unique_ptr<Expression> parseStringLiteral();
    std::unique_ptr<Expression> parseBooleanLiteral();
    std::unique_ptr<Expression> parsePrefixExpression();
    std::unique_ptr<Expression> parseInfixExpression(std::unique_ptr<Expression> left);
    std::unique_ptr<Expression> parseGroupedExpression();
    std::unique_ptr<Expression> parseIfExpression();
    std::unique_ptr<Expression> parseFunctionLiteral();
    std::unique_ptr<Expression> parseCallExpression(std::unique_ptr<Expression> function);
    
    // Helper functions
    std::unique_ptr<BlockStatement> parseBlockStatement();
    std::vector<std::unique_ptr<Identifier>> parseFunctionParameters();
    std::vector<std::unique_ptr<Expression>> parseCallArguments();
    
    // Registration functions
    void registerPrefix(TokenType type, prefixParseFn fn);
    void registerInfix(TokenType type, infixParseFn fn);

public:
    explicit Parser(Lexer lexer);
    std::unique_ptr<Program> ParseProgram();
    const std::vector<std::string>& getErrors() const { return errors; }
    bool hasErrors() const { return !errors.empty(); }
};

} // namespace script_lang

#endif // PARSER_H