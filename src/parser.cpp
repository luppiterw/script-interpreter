#include "parser.h"
#include <iostream>

namespace script_lang {

Parser::Parser(Lexer lexer) : lexer(std::move(lexer)) {
    // Initialize precedences
    precedences = {
        {TokenType::EQUAL, Precedence::EQUALS},
        {TokenType::NOT_EQUAL, Precedence::EQUALS},
        {TokenType::LESS, Precedence::LESSGREATER},
        {TokenType::LESS_EQUAL, Precedence::LESSGREATER},
        {TokenType::GREATER, Precedence::LESSGREATER},
        {TokenType::GREATER_EQUAL, Precedence::LESSGREATER},
        {TokenType::PLUS, Precedence::SUM},
        {TokenType::MINUS, Precedence::SUM},
        {TokenType::MULTIPLY, Precedence::PRODUCT},
        {TokenType::DIVIDE, Precedence::PRODUCT},
        {TokenType::MODULO, Precedence::PRODUCT},
        {TokenType::LEFT_PAREN, Precedence::CALL}
    };
    
    // Register prefix parsing functions
    registerPrefix(TokenType::IDENTIFIER, [this]() { return parseIdentifier(); });
    registerPrefix(TokenType::INTEGER, [this]() { return parseIntegerLiteral(); });
    registerPrefix(TokenType::FLOAT, [this]() { return parseFloatLiteral(); });
    registerPrefix(TokenType::STRING, [this]() { return parseStringLiteral(); });
    registerPrefix(TokenType::TRUE, [this]() { return parseBooleanLiteral(); });
    registerPrefix(TokenType::FALSE, [this]() { return parseBooleanLiteral(); });
    registerPrefix(TokenType::NOT, [this]() { return parsePrefixExpression(); });
    registerPrefix(TokenType::MINUS, [this]() { return parsePrefixExpression(); });
    registerPrefix(TokenType::LEFT_PAREN, [this]() { return parseGroupedExpression(); });
    registerPrefix(TokenType::IF, [this]() { return parseIfExpression(); });
    registerPrefix(TokenType::FUNCTION, [this]() { return parseFunctionLiteral(); });
    
    // Register infix parsing functions
    registerInfix(TokenType::PLUS, [this](auto left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::MINUS, [this](auto left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::MULTIPLY, [this](auto left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::DIVIDE, [this](auto left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::MODULO, [this](auto left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::EQUAL, [this](auto left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::NOT_EQUAL, [this](auto left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::LESS, [this](auto left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::LESS_EQUAL, [this](auto left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::GREATER, [this](auto left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::GREATER_EQUAL, [this](auto left) { return parseInfixExpression(std::move(left)); });
    registerInfix(TokenType::LEFT_PAREN, [this](auto left) { return parseCallExpression(std::move(left)); });
    
    nextToken();
    nextToken();
}

void Parser::nextToken() {
    curToken = peekToken;
    peekToken = lexer.nextToken();
}

bool Parser::curTokenIs(TokenType type) const {
    return curToken.type == type;
}

bool Parser::peekTokenIs(TokenType type) const {
    return peekToken.type == type;
}

bool Parser::expectPeek(TokenType type) {
    if (peekTokenIs(type)) {
        nextToken();
        return true;
    } else {
        peekError(type);
        return false;
    }
}

void Parser::peekError(TokenType type) {
    std::string msg = "expected next token to be " + tokenTypeToString(type) + 
                     ", got " + tokenTypeToString(peekToken.type) + 
                     " instead at line " + std::to_string(peekToken.line) + 
                     ", column " + std::to_string(peekToken.column);
    errors.push_back(msg);
}

Precedence Parser::curPrecedence() const {
    auto it = precedences.find(curToken.type);
    if (it != precedences.end()) {
        return it->second;
    }
    return Precedence::LOWEST;
}

Precedence Parser::peekPrecedence() const {
    auto it = precedences.find(peekToken.type);
    if (it != precedences.end()) {
        return it->second;
    }
    return Precedence::LOWEST;
}

std::unique_ptr<Program> Parser::ParseProgram() {
    auto program = std::make_unique<Program>();
    
    while (!curTokenIs(TokenType::EOF_)) {
        auto stmt = parseStatement();
        if (stmt) {
            program->statements.push_back(std::move(stmt));
        }
        nextToken();
    }
    
    return program;
}

std::unique_ptr<Statement> Parser::parseStatement() {
    switch (curToken.type) {
        case TokenType::LET:
            return parseLetStatement();
        case TokenType::RETURN:
            return parseReturnStatement();
        default:
            return parseExpressionStatement();
    }
}

std::unique_ptr<LetStatement> Parser::parseLetStatement() {
    auto stmt = std::make_unique<LetStatement>(curToken);
    
    if (!expectPeek(TokenType::IDENTIFIER)) {
        return nullptr;
    }
    
    stmt->name = std::make_unique<Identifier>(curToken);
    
    if (!expectPeek(TokenType::ASSIGN)) {
        return nullptr;
    }
    
    nextToken();
    stmt->value = parseExpression(Precedence::LOWEST);
    
    if (peekTokenIs(TokenType::SEMICOLON)) {
        nextToken();
    }
    
    return stmt;
}

std::unique_ptr<ReturnStatement> Parser::parseReturnStatement() {
    auto stmt = std::make_unique<ReturnStatement>(curToken);
    
    nextToken();
    
    if (!curTokenIs(TokenType::SEMICOLON)) {
        stmt->returnValue = parseExpression(Precedence::LOWEST);
    }
    
    if (peekTokenIs(TokenType::SEMICOLON)) {
        nextToken();
    }
    
    return stmt;
}

std::unique_ptr<ExpressionStatement> Parser::parseExpressionStatement() {
    auto stmt = std::make_unique<ExpressionStatement>(curToken);
    stmt->expression = parseExpression(Precedence::LOWEST);
    
    if (peekTokenIs(TokenType::SEMICOLON)) {
        nextToken();
    }
    
    return stmt;
}

std::unique_ptr<Expression> Parser::parseExpression(Precedence precedence) {
    auto prefix = prefixParseFns.find(curToken.type);
    if (prefix == prefixParseFns.end()) {
        errors.push_back("no prefix parse function for " + tokenTypeToString(curToken.type));
        return nullptr;
    }
    
    auto leftExp = prefix->second();
    
    while (!peekTokenIs(TokenType::SEMICOLON) && precedence < peekPrecedence()) {
        auto infix = infixParseFns.find(peekToken.type);
        if (infix == infixParseFns.end()) {
            return leftExp;
        }
        
        nextToken();
        leftExp = infix->second(std::move(leftExp));
    }
    
    return leftExp;
}

std::unique_ptr<Expression> Parser::parseIdentifier() {
    return std::make_unique<Identifier>(curToken);
}

std::unique_ptr<Expression> Parser::parseIntegerLiteral() {
    return std::make_unique<IntegerLiteral>(curToken);
}

std::unique_ptr<Expression> Parser::parseFloatLiteral() {
    return std::make_unique<FloatLiteral>(curToken);
}

std::unique_ptr<Expression> Parser::parseStringLiteral() {
    return std::make_unique<StringLiteral>(curToken);
}

std::unique_ptr<Expression> Parser::parseBooleanLiteral() {
    return std::make_unique<BooleanLiteral>(curToken);
}

std::unique_ptr<Expression> Parser::parsePrefixExpression() {
    auto expression = std::make_unique<PrefixExpression>(curToken, curToken.literal);
    nextToken();
    expression->right = parseExpression(Precedence::PREFIX);
    return expression;
}

std::unique_ptr<Expression> Parser::parseInfixExpression(std::unique_ptr<Expression> left) {
    auto expression = std::make_unique<InfixExpression>(curToken, std::move(left), curToken.literal);
    Precedence precedence = curPrecedence();
    nextToken();
    expression->right = parseExpression(precedence);
    return expression;
}

std::unique_ptr<Expression> Parser::parseGroupedExpression() {
    nextToken();
    auto exp = parseExpression(Precedence::LOWEST);
    
    if (!expectPeek(TokenType::RIGHT_PAREN)) {
        return nullptr;
    }
    
    return exp;
}

std::unique_ptr<Expression> Parser::parseIfExpression() {
    auto expression = std::make_unique<IfExpression>(curToken);
    
    if (!expectPeek(TokenType::LEFT_PAREN)) {
        return nullptr;
    }
    
    nextToken();
    expression->condition = parseExpression(Precedence::LOWEST);
    
    if (!expectPeek(TokenType::RIGHT_PAREN)) {
        return nullptr;
    }
    
    if (!expectPeek(TokenType::LEFT_BRACE)) {
        return nullptr;
    }
    
    expression->consequence = parseBlockStatement();
    
    if (peekTokenIs(TokenType::ELSE)) {
        nextToken();
        if (!expectPeek(TokenType::LEFT_BRACE)) {
            return nullptr;
        }
        expression->alternative = parseBlockStatement();
    }
    
    return expression;
}

std::unique_ptr<BlockStatement> Parser::parseBlockStatement() {
    auto block = std::make_unique<BlockStatement>(curToken);
    nextToken();
    
    while (!curTokenIs(TokenType::RIGHT_BRACE) && !curTokenIs(TokenType::EOF_)) {
        auto stmt = parseStatement();
        if (stmt) {
            block->statements.push_back(std::move(stmt));
        }
        nextToken();
    }
    
    return block;
}

std::unique_ptr<Expression> Parser::parseFunctionLiteral() {
    auto lit = std::make_unique<FunctionLiteral>(curToken);
    
    if (!expectPeek(TokenType::LEFT_PAREN)) {
        return nullptr;
    }
    
    lit->parameters = parseFunctionParameters();
    
    if (!expectPeek(TokenType::LEFT_BRACE)) {
        return nullptr;
    }
    
    lit->body = parseBlockStatement();
    
    return lit;
}

std::vector<std::unique_ptr<Identifier>> Parser::parseFunctionParameters() {
    std::vector<std::unique_ptr<Identifier>> identifiers;
    
    if (peekTokenIs(TokenType::RIGHT_PAREN)) {
        nextToken();
        return identifiers;
    }
    
    nextToken();
    auto ident = std::make_unique<Identifier>(curToken);
    identifiers.push_back(std::move(ident));
    
    while (peekTokenIs(TokenType::COMMA)) {
        nextToken();
        nextToken();
        auto ident = std::make_unique<Identifier>(curToken);
        identifiers.push_back(std::move(ident));
    }
    
    if (!expectPeek(TokenType::RIGHT_PAREN)) {
        return std::vector<std::unique_ptr<Identifier>>();
    }
    
    return identifiers;
}

std::unique_ptr<Expression> Parser::parseCallExpression(std::unique_ptr<Expression> function) {
    auto exp = std::make_unique<CallExpression>(curToken, std::move(function));
    exp->arguments = parseCallArguments();
    return exp;
}

std::vector<std::unique_ptr<Expression>> Parser::parseCallArguments() {
    std::vector<std::unique_ptr<Expression>> args;
    
    if (peekTokenIs(TokenType::RIGHT_PAREN)) {
        nextToken();
        return args;
    }
    
    nextToken();
    args.push_back(parseExpression(Precedence::LOWEST));
    
    while (peekTokenIs(TokenType::COMMA)) {
        nextToken();
        nextToken();
        args.push_back(parseExpression(Precedence::LOWEST));
    }
    
    if (!expectPeek(TokenType::RIGHT_PAREN)) {
        return std::vector<std::unique_ptr<Expression>>();
    }
    
    return args;
}

void Parser::registerPrefix(TokenType type, prefixParseFn fn) {
    prefixParseFns[type] = std::move(fn);
}

void Parser::registerInfix(TokenType type, infixParseFn fn) {
    infixParseFns[type] = std::move(fn);
}

} // namespace script_lang