#include "../include/lexer.h"
#include "../include/token.h"
#include <cassert>
#include <iostream>
#include <vector>

using namespace script_lang;

void testNextToken() {
    std::string input = R"(let five = 5;
let ten = 10;

let add = fn(x, y) {
    x + y;
};

let result = add(five, ten);
!-/*5;
5 < 10 > 5;

if (5 < 10) {
    return true;
} else {
    return false;
}

10 == 10;
10 != 9;
)";

    std::vector<Token> tests = {
        {TokenType::LET, "let"},
        {TokenType::IDENTIFIER, "five"},
        {TokenType::ASSIGN, "="},
        {TokenType::INTEGER, "5"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::LET, "let"},
        {TokenType::IDENTIFIER, "ten"},
        {TokenType::ASSIGN, "="},
        {TokenType::INTEGER, "10"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::LET, "let"},
        {TokenType::IDENTIFIER, "add"},
        {TokenType::ASSIGN, "="},
        {TokenType::FUNCTION, "fn"},
        {TokenType::LEFT_PAREN, "("},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::COMMA, ","},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::RIGHT_PAREN, ")"},
        {TokenType::LEFT_BRACE, "{"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::PLUS, "+"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::RIGHT_BRACE, "}"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::LET, "let"},
        {TokenType::IDENTIFIER, "result"},
        {TokenType::ASSIGN, "="},
        {TokenType::IDENTIFIER, "add"},
        {TokenType::LEFT_PAREN, "("},
        {TokenType::IDENTIFIER, "five"},
        {TokenType::COMMA, ","},
        {TokenType::IDENTIFIER, "ten"},
        {TokenType::RIGHT_PAREN, ")"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::NOT, "!"},
        {TokenType::MINUS, "-"},
        {TokenType::DIVIDE, "/"},
        {TokenType::MULTIPLY, "*"},
        {TokenType::INTEGER, "5"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::INTEGER, "5"},
        {TokenType::LESS, "<"},
        {TokenType::INTEGER, "10"},
        {TokenType::GREATER, ">"},
        {TokenType::INTEGER, "5"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::IF, "if"},
        {TokenType::LEFT_PAREN, "("},
        {TokenType::INTEGER, "5"},
        {TokenType::LESS, "<"},
        {TokenType::INTEGER, "10"},
        {TokenType::RIGHT_PAREN, ")"},
        {TokenType::LEFT_BRACE, "{"},
        {TokenType::RETURN, "return"},
        {TokenType::TRUE, "true"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::RIGHT_BRACE, "}"},
        {TokenType::ELSE, "else"},
        {TokenType::LEFT_BRACE, "{"},
        {TokenType::RETURN, "return"},
        {TokenType::FALSE, "false"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::RIGHT_BRACE, "}"},
        {TokenType::INTEGER, "10"},
        {TokenType::EQUAL, "=="},
        {TokenType::INTEGER, "10"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::INTEGER, "10"},
        {TokenType::NOT_EQUAL, "!="},
        {TokenType::INTEGER, "9"},
        {TokenType::SEMICOLON, ";"},
        {TokenType::EOF_, ""}
    };

    Lexer lexer(input);
    
    for (size_t i = 0; i < tests.size(); ++i) {
        Token tok = lexer.nextToken();
        
        assert(tok.type == tests[i].type);
        assert(tok.literal == tests[i].literal);
        
        std::cout << "Test " << i << " passed: " << tokenTypeToString(tok.type) 
                  << " \"" << tok.literal << "\"" << std::endl;
    }
    
    std::cout << "All lexer tests passed!" << std::endl;
}

int main() {
    testNextToken();
    return 0;
}