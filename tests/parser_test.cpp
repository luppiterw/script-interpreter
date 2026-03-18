#include "../include/parser.h"
#include "../include/lexer.h"
#include <cassert>
#include <iostream>
#include <memory>

using namespace script_lang;

void testLetStatements() {
    std::string input = R"(
let x = 5;
let y = 10;
let foobar = 838383;
)";

    Lexer lexer(input);
    Parser parser(std::move(lexer));
    auto program = parser.ParseProgram();
    
    assert(!parser.hasErrors());
    assert(program->statements.size() == 3);
    
    std::vector<std::string> expectedIdentifiers = {"x", "y", "foobar"};
    
    for (size_t i = 0; i < expectedIdentifiers.size(); ++i) {
        auto letStmt = dynamic_cast<LetStatement*>(program->statements[i].get());
        assert(letStmt != nullptr);
        assert(letStmt->name->value == expectedIdentifiers[i]);
        assert(letStmt->name->tokenLiteral() == expectedIdentifiers[i]);
    }
    
    std::cout << "Let statements test passed!" << std::endl;
}

void testReturnStatements() {
    std::string input = R"(
return 5;
return 10;
return 993322;
)";

    Lexer lexer(input);
    Parser parser(std::move(lexer));
    auto program = parser.ParseProgram();
    
    assert(!parser.hasErrors());
    assert(program->statements.size() == 3);
    
    for (const auto& stmt : program->statements) {
        auto returnStmt = dynamic_cast<ReturnStatement*>(stmt.get());
        assert(returnStmt != nullptr);
        assert(returnStmt->tokenLiteral() == "return");
    }
    
    std::cout << "Return statements test passed!" << std::endl;
}

void testIdentifierExpression() {
    std::string input = "foobar;";
    
    Lexer lexer(input);
    Parser parser(std::move(lexer));
    auto program = parser.ParseProgram();
    
    assert(!parser.hasErrors());
    assert(program->statements.size() == 1);
    
    auto stmt = dynamic_cast<ExpressionStatement*>(program->statements[0].get());
    assert(stmt != nullptr);
    
    auto ident = dynamic_cast<Identifier*>(stmt->expression.get());
    assert(ident != nullptr);
    assert(ident->value == "foobar");
    assert(ident->tokenLiteral() == "foobar");
    
    std::cout << "Identifier expression test passed!" << std::endl;
}

void testIntegerLiteralExpression() {
    std::string input = "5;";
    
    Lexer lexer(input);
    Parser parser(std::move(lexer));
    auto program = parser.ParseProgram();
    
    assert(!parser.hasErrors());
    assert(program->statements.size() == 1);
    
    auto stmt = dynamic_cast<ExpressionStatement*>(program->statements[0].get());
    assert(stmt != nullptr);
    
    auto literal = dynamic_cast<IntegerLiteral*>(stmt->expression.get());
    assert(literal != nullptr);
    assert(literal->value == 5);
    assert(literal->tokenLiteral() == "5");
    
    std::cout << "Integer literal test passed!" << std::endl;
}

void testParsingPrefixExpressions() {
    struct PrefixTest {
        std::string input;
        std::string op;
        int value;
    };
    
    std::vector<PrefixTest> prefixTests = {
        {"!5;", "!", 5},
        {"-15;", "-", 15}
    };
    
    for (const auto& test : prefixTests) {
        Lexer lexer(test.input);
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        assert(!parser.hasErrors());
        assert(program->statements.size() == 1);
        
        auto stmt = dynamic_cast<ExpressionStatement*>(program->statements[0].get());
        assert(stmt != nullptr);
        
        auto exp = dynamic_cast<PrefixExpression*>(stmt->expression.get());
        assert(exp != nullptr);
        assert(exp->op == test.op);
        
        auto literal = dynamic_cast<IntegerLiteral*>(exp->right.get());
        assert(literal != nullptr);
        assert(literal->value == test.value);
    }
    
    std::cout << "Prefix expressions test passed!" << std::endl;
}

void testParsingInfixExpressions() {
    struct InfixTest {
        std::string input;
        int leftValue;
        std::string op;
        int rightValue;
    };
    
    std::vector<InfixTest> infixTests = {
        {"5 + 5;", 5, "+", 5},
        {"5 - 5;", 5, "-", 5},
        {"5 * 5;", 5, "*", 5},
        {"5 / 5;", 5, "/", 5},
        {"5 > 5;", 5, ">", 5},
        {"5 < 5;", 5, "<", 5},
        {"5 == 5;", 5, "==", 5},
        {"5 != 5;", 5, "!=", 5}
    };
    
    for (const auto& test : infixTests) {
        Lexer lexer(test.input);
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        assert(!parser.hasErrors());
        assert(program->statements.size() == 1);
        
        auto stmt = dynamic_cast<ExpressionStatement*>(program->statements[0].get());
        assert(stmt != nullptr);
        
        auto exp = dynamic_cast<InfixExpression*>(stmt->expression.get());
        assert(exp != nullptr);
        
        auto left = dynamic_cast<IntegerLiteral*>(exp->left.get());
        assert(left != nullptr);
        assert(left->value == test.leftValue);
        
        assert(exp->op == test.op);
        
        auto right = dynamic_cast<IntegerLiteral*>(exp->right.get());
        assert(right != nullptr);
        assert(right->value == test.rightValue);
    }
    
    std::cout << "Infix expressions test passed!" << std::endl;
}

void testOperatorPrecedenceParsing() {
    struct PrecedenceTest {
        std::string input;
        std::string expected;
    };
    
    std::vector<PrecedenceTest> tests = {
        {"-a * b", "((-a) * b)"},
        {"!-a;", "(!(-a))"},
        {"a + b + c", "((a + b) + c)"},
        {"a + b - c", "((a + b) - c)"},
        {"a * b * c", "((a * b) * c)"},
        {"a * b / c", "((a * b) / c)"},
        {"a + b / c", "(a + (b / c))"},
        {"a + b * c + d / e - f", "(((a + (b * c)) + (d / e)) - f)"},
        {"3 + 4; -5 * 5", "(3 + 4)((-5) * 5)"},
        {"5 > 4 == 3 < 4", "((5 > 4) == (3 < 4))"},
        {"5 < 4 != 3 > 4", "((5 < 4) != (3 > 4))"},
        {"3 + 4 * 5 == 3 * 1 + 4 * 5", "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))"}
    };
    
    for (const auto& test : tests) {
        Lexer lexer(test.input);
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        assert(!parser.hasErrors());
        
        std::string actual = program->toString();
        // Remove semicolons for comparison
        actual.erase(std::remove(actual.begin(), actual.end(), ';'), actual.end());
        
        assert(actual == test.expected);
    }
    
    std::cout << "Operator precedence test passed!" << std::endl;
}

void testBooleanExpression() {
    struct BooleanTest {
        std::string input;
        bool expectedBoolean;
    };
    
    std::vector<BooleanTest> tests = {
        {"true;", true},
        {"false;", false}
    };
    
    for (const auto& test : tests) {
        Lexer lexer(test.input);
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        assert(!parser.hasErrors());
        assert(program->statements.size() == 1);
        
        auto stmt = dynamic_cast<ExpressionStatement*>(program->statements[0].get());
        assert(stmt != nullptr);
        
        auto boolean = dynamic_cast<BooleanLiteral*>(stmt->expression.get());
        assert(boolean != nullptr);
        assert(boolean->value == test.expectedBoolean);
    }
    
    std::cout << "Boolean expression test passed!" << std::endl;
}

void testIfExpression() {
    std::string input = "if (x < y) { x }";
    
    Lexer lexer(input);
    Parser parser(std::move(lexer));
    auto program = parser.ParseProgram();
    
    assert(!parser.hasErrors());
    assert(program->statements.size() == 1);
    
    auto stmt = dynamic_cast<ExpressionStatement*>(program->statements[0].get());
    assert(stmt != nullptr);
    
    auto exp = dynamic_cast<IfExpression*>(stmt->expression.get());
    assert(exp != nullptr);
    
    auto cond = dynamic_cast<InfixExpression*>(exp->condition.get());
    assert(cond != nullptr);
    
    auto left = dynamic_cast<Identifier*>(cond->left.get());
    assert(left != nullptr);
    assert(left->value == "x");
    
    assert(cond->op == "<");
    
    auto right = dynamic_cast<Identifier*>(cond->right.get());
    assert(right != nullptr);
    assert(right->value == "y");
    
    auto consequence = exp->consequence.get();
    assert(consequence != nullptr);
    assert(consequence->statements.size() == 1);
    
    auto consequenceStmt = dynamic_cast<ExpressionStatement*>(consequence->statements[0].get());
    assert(consequenceStmt != nullptr);
    
    auto ident = dynamic_cast<Identifier*>(consequenceStmt->expression.get());
    assert(ident != nullptr);
    assert(ident->value == "x");
    
    assert(exp->alternative == nullptr);
    
    std::cout << "If expression test passed!" << std::endl;
}

void testIfElseExpression() {
    std::string input = "if (x < y) { x } else { y }";
    
    Lexer lexer(input);
    Parser parser(std::move(lexer));
    auto program = parser.ParseProgram();
    
    assert(!parser.hasErrors());
    assert(program->statements.size() == 1);
    
    auto stmt = dynamic_cast<ExpressionStatement*>(program->statements[0].get());
    assert(stmt != nullptr);
    
    auto exp = dynamic_cast<IfExpression*>(stmt->expression.get());
    assert(exp != nullptr);
    
    assert(exp->alternative != nullptr);
    assert(exp->alternative->statements.size() == 1);
    
    auto alternativeStmt = dynamic_cast<ExpressionStatement*>(exp->alternative->statements[0].get());
    assert(alternativeStmt != nullptr);
    
    auto ident = dynamic_cast<Identifier*>(alternativeStmt->expression.get());
    assert(ident != nullptr);
    assert(ident->value == "y");
    
    std::cout << "If-else expression test passed!" << std::endl;
}

void testFunctionLiteralParsing() {
    std::string input = "fn(x, y) { x + y; }";
    
    Lexer lexer(input);
    Parser parser(std::move(lexer));
    auto program = parser.ParseProgram();
    
    assert(!parser.hasErrors());
    assert(program->statements.size() == 1);
    
    auto stmt = dynamic_cast<ExpressionStatement*>(program->statements[0].get());
    assert(stmt != nullptr);
    
    auto function = dynamic_cast<FunctionLiteral*>(stmt->expression.get());
    assert(function != nullptr);
    assert(function->parameters.size() == 2);
    
    assert(function->parameters[0]->value == "x");
    assert(function->parameters[1]->value == "y");
    
    assert(function->body->statements.size() == 1);
    
    auto bodyStmt = dynamic_cast<ExpressionStatement*>(function->body->statements[0].get());
    assert(bodyStmt != nullptr);
    
    auto bodyExp = dynamic_cast<InfixExpression*>(bodyStmt->expression.get());
    assert(bodyExp != nullptr);
    
    auto left = dynamic_cast<Identifier*>(bodyExp->left.get());
    assert(left != nullptr);
    assert(left->value == "x");
    
    assert(bodyExp->op == "+");
    
    auto right = dynamic_cast<Identifier*>(bodyExp->right.get());
    assert(right != nullptr);
    assert(right->value == "y");
    
    std::cout << "Function literal parsing test passed!" << std::endl;
}

void testFunctionParameterParsing() {
    struct ParameterTest {
        std::string input;
        std::vector<std::string> expectedParams;
    };
    
    std::vector<ParameterTest> tests = {
        {"fn() {};", {}},
        {"fn(x) {};", {"x"}},
        {"fn(x, y, z) {};", {"x", "y", "z"}}
    };
    
    for (const auto& test : tests) {
        Lexer lexer(test.input);
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        assert(!parser.hasErrors());
        assert(program->statements.size() == 1);
        
        auto stmt = dynamic_cast<ExpressionStatement*>(program->statements[0].get());
        assert(stmt != nullptr);
        
        auto function = dynamic_cast<FunctionLiteral*>(stmt->expression.get());
        assert(function != nullptr);
        assert(function->parameters.size() == test.expectedParams.size());
        
        for (size_t i = 0; i < test.expectedParams.size(); ++i) {
            assert(function->parameters[i]->value == test.expectedParams[i]);
        }
    }
    
    std::cout << "Function parameter parsing test passed!" << std::endl;
}

void testCallExpressionParsing() {
    std::string input = "add(1, 2 * 3, 4 + 5);";
    
    Lexer lexer(input);
    Parser parser(std::move(lexer));
    auto program = parser.ParseProgram();
    
    assert(!parser.hasErrors());
    assert(program->statements.size() == 1);
    
    auto stmt = dynamic_cast<ExpressionStatement*>(program->statements[0].get());
    assert(stmt != nullptr);
    
    auto exp = dynamic_cast<CallExpression*>(stmt->expression.get());
    assert(exp != nullptr);
    assert(exp->arguments.size() == 3);
    
    auto ident = dynamic_cast<Identifier*>(exp->function.get());
    assert(ident != nullptr);
    assert(ident->value == "add");
    
    auto firstArg = dynamic_cast<IntegerLiteral*>(exp->arguments[0].get());
    assert(firstArg != nullptr);
    assert(firstArg->value == 1);
    
    auto secondArg = dynamic_cast<InfixExpression*>(exp->arguments[1].get());
    assert(secondArg != nullptr);
    assert(secondArg->op == "*");
    
    auto thirdArg = dynamic_cast<InfixExpression*>(exp->arguments[2].get());
    assert(thirdArg != nullptr);
    assert(thirdArg->op == "+");
    
    std::cout << "Call expression parsing test passed!" << std::endl;
}

int main() {
    testLetStatements();
    testReturnStatements();
    testIdentifierExpression();
    testIntegerLiteralExpression();
    testParsingPrefixExpressions();
    testParsingInfixExpressions();
    testOperatorPrecedenceParsing();
    testBooleanExpression();
    testIfExpression();
    testIfElseExpression();
    testFunctionLiteralParsing();
    testFunctionParameterParsing();
    testCallExpressionParsing();
    
    std::cout << "\nAll parser tests passed!" << std::endl;
    return 0;
}