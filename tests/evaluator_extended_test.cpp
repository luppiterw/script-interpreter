#include "../include/evaluator.h"
#include "../include/parser.h"
#include "../include/lexer.h"
#include <cassert>
#include <iostream>
#include <memory>
#include <cmath>

using namespace script_lang;

void testNestedArithmetic() {
    std::cout << "Testing nested arithmetic expressions..." << std::endl;
    
    struct TestCase {
        std::string input;
        int expected;
    };
    
    std::vector<TestCase> tests = {
        {"(5-(2-44))*12/(22/2-1)", 56},
        {"((1+2)*3)", 9},
        {"1+2*3", 7},
        {"(1+2)*3", 9},
        {"10/(2+3)", 2},
        {"(10+5)*(2-1)", 15},
        {"100-50-25", 25},
        {"100/10/2", 5},
        {"2*3+4*5", 26},
        {"2*(3+4)*5", 70},
        {"-5+10", 5},
        {"--5", 5},
        {"5%-3", 2},
        {"-5%3", -2}
    };
    
    for (const auto& test : tests) {
        Lexer lexer(test.input + ";");
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        Evaluator evaluator;
        auto evaluated = evaluator.evaluate(program.get());
        
        auto integer = std::dynamic_pointer_cast<Integer>(evaluated);
        assert(integer != nullptr);
        assert(integer->value == test.expected);
        
        std::cout << "  " << test.input << " = " << test.expected << "" << std::endl;
    }
    
    std::cout << "  Nested arithmetic tests passed!" << std::endl;
}

void testFloatOperations() {
    std::cout << "Testing float operations..." << std::endl;
    
    struct TestCase {
        std::string input;
        double expected;
    };
    
    std::vector<TestCase> tests = {
        {"3.14 + 2.86", 6.0},
        {"10.0 - 3.5", 6.5},
        {"2.5 * 4.0", 10.0},
        {"7.5 / 2.5", 3.0},
        {"3.14 * 2.0", 6.28},
        {"-2.5", -2.5}
    };
    
    for (const auto& test : tests) {
        Lexer lexer(test.input + ";");
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        Evaluator evaluator;
        auto evaluated = evaluator.evaluate(program.get());
        
        auto flt = std::dynamic_pointer_cast<Float>(evaluated);
        assert(flt != nullptr);
        assert(std::abs(flt->value - test.expected) < 0.001);
        
        std::cout << "  " << test.input << " = " << test.expected << "" << std::endl;
    }
    
    std::cout << "  Float operations tests passed!" << std::endl;
}

void testComparisonOperators() {
    std::cout << "Testing comparison operators..." << std::endl;
    
    struct TestCase {
        std::string input;
        bool expected;
    };
    
    std::vector<TestCase> tests = {
        {"5 == 5", true},
        {"5 != 5", false},
        {"5 < 10", true},
        {"5 > 10", false},
        {"5 <= 5", true},
        {"5 >= 5", true},
        {"5 <= 4", false},
        {"5 >= 6", false},
        {"3.14 > 3.0", true},
        {"3.14 < 4.0", true},
        {"\"a\" == \"a\"", true},
        {"\"a\" != \"b\"", true},
        {"true == true", true},
        {"true == false", false},
        {"true != false", true}
    };
    
    for (const auto& test : tests) {
        Lexer lexer(test.input + ";");
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        Evaluator evaluator;
        auto evaluated = evaluator.evaluate(program.get());
        
        auto boolean = std::dynamic_pointer_cast<Boolean>(evaluated);
        assert(boolean != nullptr);
        assert(boolean->value == test.expected);
        
        std::cout << "  " << test.input << " = " << (test.expected ? "true" : "false") << "" << std::endl;
    }
    
    std::cout << "  Comparison operators tests passed!" << std::endl;
}

void testStringOperations() {
    std::cout << "Testing string operations..." << std::endl;
    
    // String concatenation
    {
        Lexer lexer("\"Hello\" + \" \" + \"World\";");
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        Evaluator evaluator;
        auto evaluated = evaluator.evaluate(program.get());
        
        auto str = std::dynamic_pointer_cast<String>(evaluated);
        assert(str != nullptr);
        assert(str->value == "Hello World");
        std::cout << "  String concatenation" << std::endl;
    }
    
    // String length
    {
        Lexer lexer("len(\"Hello\");");
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        Evaluator evaluator;
        auto evaluated = evaluator.evaluate(program.get());
        
        auto integer = std::dynamic_pointer_cast<Integer>(evaluated);
        assert(integer != nullptr);
        assert(integer->value == 5);
        std::cout << "  String length" << std::endl;
    }
    
    std::cout << "  String operations tests passed!" << std::endl;
}

void testClosures() {
    std::cout << "Testing closures..." << std::endl;
    
    // Simple closure
    std::string input = R"(
        let makeAdder = fn(x) {
            return fn(y) { x + y; };
        };
        let add5 = makeAdder(5);
        add5(10);
    )";
    
    Lexer lexer(input);
    Parser parser(std::move(lexer));
    auto program = parser.ParseProgram();
    
    Evaluator evaluator;
    auto evaluated = evaluator.evaluate(program.get());
    
    auto integer = std::dynamic_pointer_cast<Integer>(evaluated);
    assert(integer != nullptr);
    assert(integer->value == 15);
    
    std::cout << "  Closure test passed!" << std::endl;
}

void testRecursion() {
    std::cout << "Testing recursion..." << std::endl;
    
    // Factorial
    std::string input = R"(
        let factorial = fn(n) {
            if (n <= 1) {
                return 1;
            }
            return n * factorial(n - 1);
        };
        factorial(6);
    )";
    
    Lexer lexer(input);
    Parser parser(std::move(lexer));
    auto program = parser.ParseProgram();
    
    Evaluator evaluator;
    auto evaluated = evaluator.evaluate(program.get());
    
    auto integer = std::dynamic_pointer_cast<Integer>(evaluated);
    assert(integer != nullptr);
    assert(integer->value == 720);
    
    std::cout << "  Recursion test passed!" << std::endl;
}

void testHigherOrderFunctions() {
    std::cout << "Testing higher-order functions..." << std::endl;
    
    // Map-like function
    std::string input = R"(
        let twice = fn(x) { x * 2; };
        let apply = fn(f, x) { f(x); };
        apply(twice, 21);
    )";
    
    Lexer lexer(input);
    Parser parser(std::move(lexer));
    auto program = parser.ParseProgram();
    
    Evaluator evaluator;
    auto evaluated = evaluator.evaluate(program.get());
    
    auto integer = std::dynamic_pointer_cast<Integer>(evaluated);
    assert(integer != nullptr);
    assert(integer->value == 42);
    
    std::cout << "  Higher-order functions test passed!" << std::endl;
}

void testTruthyValues() {
    std::cout << "Testing truthy values..." << std::endl;
    
    struct TestCase {
        std::string input;
        std::string expected;
    };
    
    std::vector<TestCase> tests = {
        {"if (1) { \"yes\" } else { \"no\" }", "yes"},
        {"if (0) { \"yes\" } else { \"no\" }", "no"},
        {"if (-5) { \"yes\" } else { \"no\" }", "yes"},
        {"if (\"\") { \"yes\" } else { \"no\" }", "no"},
        {"if (\"hello\") { \"yes\" } else { \"no\" }", "yes"},
        {"if (true) { \"yes\" } else { \"no\" }", "yes"},
        {"if (false) { \"yes\" } else { \"no\" }", "no"},
        {"if (null) { \"yes\" } else { \"no\" }", "no"}
    };
    
    for (const auto& test : tests) {
        Lexer lexer(test.input + ";");
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        Evaluator evaluator;
        auto evaluated = evaluator.evaluate(program.get());
        
        auto str = std::dynamic_pointer_cast<String>(evaluated);
        assert(str != nullptr);
        assert(str->value == test.expected);
    }
    
    std::cout << "  Truthy values tests passed!" << std::endl;
}

void testBuiltinFunctions() {
    std::cout << "Testing built-in functions..." << std::endl;
    
    // len with string
    {
        Lexer lexer("len(\"hello world\");");
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        Evaluator evaluator;
        auto evaluated = evaluator.evaluate(program.get());
        
        auto integer = std::dynamic_pointer_cast<Integer>(evaluated);
        assert(integer != nullptr);
        assert(integer->value == 11);
    }
    
    // puts (returns null)
    {
        Lexer lexer("puts(\"test\");");
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        Evaluator evaluator;
        auto evaluated = evaluator.evaluate(program.get());
        
        auto nullObj = std::dynamic_pointer_cast<Null>(evaluated);
        assert(nullObj != nullptr);
    }
    
    std::cout << "  Built-in functions tests passed!" << std::endl;
}

void testErrorHandling() {
    std::cout << "Testing error handling..." << std::endl;
    
    struct TestCase {
        std::string input;
        bool shouldError;
    };
    
    std::vector<TestCase> tests = {
        {"5 + true;", true},           // Type mismatch
        {"-true;", true},              // Invalid prefix operator
        {"unknownVar;", true},         // Undefined variable
        {"5 / 0;", true},              // Division by zero
        {"fn(1, 2);", true},           // Calling non-function
        {"len();", true},              // Wrong argument count
    };
    
    for (const auto& test : tests) {
        Lexer lexer(test.input);
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        Evaluator evaluator;
        auto evaluated = evaluator.evaluate(program.get());
        
        if (test.shouldError) {
            auto error = std::dynamic_pointer_cast<Error>(evaluated);
            assert(error != nullptr);
        }
    }
    
    std::cout << "  Error handling tests passed!" << std::endl;
}

void testComplexPrograms() {
    std::cout << "Testing complex programs..." << std::endl;
    
    // Fibonacci
    {
        std::string input = R"(
            let fib = fn(n) {
                if (n <= 1) {
                    return n;
                }
                return fib(n - 1) + fib(n - 2);
            };
            fib(10);
        )";
        
        Lexer lexer(input);
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        Evaluator evaluator;
        auto evaluated = evaluator.evaluate(program.get());
        
        auto integer = std::dynamic_pointer_cast<Integer>(evaluated);
        assert(integer != nullptr);
        assert(integer->value == 55);
        
        std::cout << "  Fibonacci" << std::endl;
    }
    
    // Counter factory
    {
        std::string input = R"(
            let makeCounter = fn(start) {
                return fn() {
                    return start;
                };
            };
            let counter = makeCounter(42);
            counter();
        )";
        
        Lexer lexer(input);
        Parser parser(std::move(lexer));
        auto program = parser.ParseProgram();
        
        Evaluator evaluator;
        auto evaluated = evaluator.evaluate(program.get());
        
        auto integer = std::dynamic_pointer_cast<Integer>(evaluated);
        assert(integer != nullptr);
        assert(integer->value == 42);
        
        std::cout << "  Counter factory" << std::endl;
    }
    
    std::cout << "  Complex programs tests passed!" << std::endl;
}

int main() {
    std::cout << "Running extended evaluator tests...\n" << std::endl;
    
    testNestedArithmetic();
    testFloatOperations();
    testComparisonOperators();
    testStringOperations();
    testClosures();
    testRecursion();
    testHigherOrderFunctions();
    testTruthyValues();
    testBuiltinFunctions();
    testErrorHandling();
    testComplexPrograms();
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "All extended evaluator tests passed!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}