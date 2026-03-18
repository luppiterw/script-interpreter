#include "../include/lexer.h"
#include "../include/token.h"
#include <cassert>
#include <iostream>
#include <vector>

using namespace script_lang;

void testNestedExpressions() {
    std::cout << "Testing nested expressions..." << std::endl;
    
    // Test: (5-(2-44))*12/(22/2-1) = 56
    Lexer lexer("(5-(2-44))*12/(22/2-1);");
    
    std::vector<TokenType> expected = {
        TokenType::LEFT_PAREN,
        TokenType::INTEGER,      // 5
        TokenType::MINUS,
        TokenType::LEFT_PAREN,
        TokenType::INTEGER,      // 2
        TokenType::MINUS,
        TokenType::INTEGER,      // 44
        TokenType::RIGHT_PAREN,
        TokenType::RIGHT_PAREN,
        TokenType::MULTIPLY,
        TokenType::INTEGER,      // 12
        TokenType::DIVIDE,
        TokenType::LEFT_PAREN,
        TokenType::INTEGER,      // 22
        TokenType::DIVIDE,
        TokenType::INTEGER,      // 2
        TokenType::MINUS,
        TokenType::INTEGER,      // 1
        TokenType::RIGHT_PAREN,
        TokenType::SEMICOLON,
        TokenType::EOF_
    };
    
    for (size_t i = 0; i < expected.size(); ++i) {
        Token tok = lexer.nextToken();
        assert(tok.type == expected[i]);
    }
    
    std::cout << "  Nested expressions test passed!" << std::endl;
}

void testStringEscapes() {
    std::cout << "Testing string tokens..." << std::endl;
    
    Lexer lexer("\"hello world\"; \"test\";");
    
    Token t1 = lexer.nextToken();
    assert(t1.type == TokenType::STRING);
    assert(t1.literal == "hello world");
    
    Token t2 = lexer.nextToken();
    assert(t2.type == TokenType::SEMICOLON);
    
    Token t3 = lexer.nextToken();
    assert(t3.type == TokenType::STRING);
    assert(t3.literal == "test");
    
    std::cout << "  String tokens test passed!" << std::endl;
}

void testFloats() {
    std::cout << "Testing float tokens..." << std::endl;
    
    Lexer lexer("3.14; 2.5; 0.5;");
    
    Token t1 = lexer.nextToken();
    assert(t1.type == TokenType::FLOAT);
    assert(t1.literal == "3.14");
    
    Token t2 = lexer.nextToken();
    assert(t2.type == TokenType::SEMICOLON);
    
    Token t3 = lexer.nextToken();
    assert(t3.type == TokenType::FLOAT);
    assert(t3.literal == "2.5");
    
    std::cout << "  Float tokens test passed!" << std::endl;
}

void testKeywords() {
    std::cout << "Testing keywords..." << std::endl;
    
    Lexer lexer("let fn if else while for return true false null class const");
    
    assert(lexer.nextToken().type == TokenType::LET);
    assert(lexer.nextToken().type == TokenType::FUNCTION);
    assert(lexer.nextToken().type == TokenType::IF);
    assert(lexer.nextToken().type == TokenType::ELSE);
    assert(lexer.nextToken().type == TokenType::WHILE);
    assert(lexer.nextToken().type == TokenType::FOR);
    assert(lexer.nextToken().type == TokenType::RETURN);
    assert(lexer.nextToken().type == TokenType::TRUE);
    assert(lexer.nextToken().type == TokenType::FALSE);
    assert(lexer.nextToken().type == TokenType::NULL_);
    assert(lexer.nextToken().type == TokenType::CLASS);
    assert(lexer.nextToken().type == TokenType::CONST);
    
    std::cout << "  Keywords test passed!" << std::endl;
}

void testOperators() {
    std::cout << "Testing operators..." << std::endl;
    
    Lexer lexer("+ - * / % = == != < <= > >= && || !");
    
    assert(lexer.nextToken().type == TokenType::PLUS);
    assert(lexer.nextToken().type == TokenType::MINUS);
    assert(lexer.nextToken().type == TokenType::MULTIPLY);
    assert(lexer.nextToken().type == TokenType::DIVIDE);
    assert(lexer.nextToken().type == TokenType::MODULO);
    assert(lexer.nextToken().type == TokenType::ASSIGN);
    assert(lexer.nextToken().type == TokenType::EQUAL);
    assert(lexer.nextToken().type == TokenType::NOT_EQUAL);
    assert(lexer.nextToken().type == TokenType::LESS);
    assert(lexer.nextToken().type == TokenType::LESS_EQUAL);
    assert(lexer.nextToken().type == TokenType::GREATER);
    assert(lexer.nextToken().type == TokenType::GREATER_EQUAL);
    assert(lexer.nextToken().type == TokenType::AND);
    assert(lexer.nextToken().type == TokenType::OR);
    assert(lexer.nextToken().type == TokenType::NOT);
    
    std::cout << "  Operators test passed!" << std::endl;
}

void testDelimiters() {
    std::cout << "Testing delimiters..." << std::endl;
    
    Lexer lexer("( ) { } [ ] , ; .");
    
    assert(lexer.nextToken().type == TokenType::LEFT_PAREN);
    assert(lexer.nextToken().type == TokenType::RIGHT_PAREN);
    assert(lexer.nextToken().type == TokenType::LEFT_BRACE);
    assert(lexer.nextToken().type == TokenType::RIGHT_BRACE);
    assert(lexer.nextToken().type == TokenType::LEFT_BRACKET);
    assert(lexer.nextToken().type == TokenType::RIGHT_BRACKET);
    assert(lexer.nextToken().type == TokenType::COMMA);
    assert(lexer.nextToken().type == TokenType::SEMICOLON);
    assert(lexer.nextToken().type == TokenType::DOT);
    
    std::cout << "  Delimiters test passed!" << std::endl;
}

void testComments() {
    std::cout << "Testing comments..." << std::endl;
    
    Lexer lexer("let x = 5; // this is a comment\nlet y = 10;");
    
    assert(lexer.nextToken().type == TokenType::LET);
    assert(lexer.nextToken().type == TokenType::IDENTIFIER);
    assert(lexer.nextToken().type == TokenType::ASSIGN);
    assert(lexer.nextToken().type == TokenType::INTEGER);
    assert(lexer.nextToken().type == TokenType::SEMICOLON);
    // Comment should be skipped
    assert(lexer.nextToken().type == TokenType::LET);
    assert(lexer.nextToken().type == TokenType::IDENTIFIER);
    assert(lexer.nextToken().type == TokenType::ASSIGN);
    assert(lexer.nextToken().type == TokenType::INTEGER);
    assert(lexer.nextToken().type == TokenType::SEMICOLON);
    
    std::cout << "  Comments test passed!" << std::endl;
}

void testComplexProgram() {
    std::cout << "Testing complex program..." << std::endl;
    
    std::string input = R"(
        let add = fn(a, b) {
            return a + b;
        };
        let result = add(3, 4);
    )";
    
    Lexer lexer(input);
    
    std::vector<TokenType> expected = {
        TokenType::LET,
        TokenType::IDENTIFIER,    // add
        TokenType::ASSIGN,
        TokenType::FUNCTION,
        TokenType::LEFT_PAREN,
        TokenType::IDENTIFIER,    // a
        TokenType::COMMA,
        TokenType::IDENTIFIER,    // b
        TokenType::RIGHT_PAREN,
        TokenType::LEFT_BRACE,
        TokenType::RETURN,
        TokenType::IDENTIFIER,    // a
        TokenType::PLUS,
        TokenType::IDENTIFIER,    // b
        TokenType::SEMICOLON,
        TokenType::RIGHT_BRACE,
        TokenType::SEMICOLON,
        TokenType::LET,
        TokenType::IDENTIFIER,    // result
        TokenType::ASSIGN,
        TokenType::IDENTIFIER,    // add
        TokenType::LEFT_PAREN,
        TokenType::INTEGER,       // 3
        TokenType::COMMA,
        TokenType::INTEGER,       // 4
        TokenType::RIGHT_PAREN,
        TokenType::SEMICOLON,
        TokenType::EOF_
    };
    
    for (size_t i = 0; i < expected.size(); ++i) {
        Token tok = lexer.nextToken();
        assert(tok.type == expected[i]);
    }
    
    std::cout << "  Complex program test passed!" << std::endl;
}

void testEdgeCases() {
    std::cout << "Testing edge cases..." << std::endl;
    
    // Empty input
    Lexer lexer1("");
    assert(lexer1.nextToken().type == TokenType::EOF_);
    
    // Only whitespace
    Lexer lexer2("   \t\n  ");
    assert(lexer2.nextToken().type == TokenType::EOF_);
    
    // Multiple semicolons
    Lexer lexer3(";;;");
    assert(lexer3.nextToken().type == TokenType::SEMICOLON);
    assert(lexer3.nextToken().type == TokenType::SEMICOLON);
    assert(lexer3.nextToken().type == TokenType::SEMICOLON);
    
    // Negative numbers (actually prefix minus)
    Lexer lexer4("-5");
    assert(lexer4.nextToken().type == TokenType::MINUS);
    assert(lexer4.nextToken().type == TokenType::INTEGER);
    
    std::cout << "  Edge cases test passed!" << std::endl;
}

int main() {
    std::cout << "Running extended lexer tests...\n" << std::endl;
    
    testNestedExpressions();
    testStringEscapes();
    testFloats();
    testKeywords();
    testOperators();
    testDelimiters();
    testComments();
    testComplexProgram();
    testEdgeCases();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "All extended lexer tests passed!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}