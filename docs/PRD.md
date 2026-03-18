# Product Requirements Document (PRD)

## Script Interpreter

**Version:** 1.0.0  
**Status:** In Development  
**Author:** luppiterw  
**Last Updated:** 2026-03-18

---

## 1. Executive Summary

A lightweight, embeddable scripting language interpreter implemented in C++17. The interpreter provides a simple yet powerful scripting capability with support for variables, functions, control structures, and object-oriented programming features.

---

## 2. Goals

### 2.1 Primary Goals

- Provide a clean, well-documented C++17 scripting language implementation
- Support core programming constructs (variables, functions, control flow)
- Enable easy embedding into other C++ applications
- Maintain high code quality with comprehensive test coverage

### 2.2 Non-Goals

- JIT compilation or bytecode generation
- Performance optimization for production workloads
- Standard library implementation beyond basic built-ins

---

## 3. Target Users

| User Type | Description | Use Case |
|-----------|-------------|----------|
| Game Developers | Need embedded scripting for game logic | NPC behavior, quest systems |
| Application Developers | Need plugin/extension scripting | User customization, automation |
| Language Enthusiasts | Learning interpreter implementation | Educational purposes |
| Embedded Developers | Need lightweight scripting | Configuration, automation |

---

## 4. Feature Requirements

### 4.1 Core Features (MVP) ✅

| Feature | Priority | Status | Description |
|---------|----------|--------|-------------|
| Integer Type | P0 | ✅ Done | 32-bit signed integers |
| Float Type | P0 | ✅ Done | 64-bit floating point |
| String Type | P0 | ✅ Done | UTF-8 string support |
| Boolean Type | P0 | ✅ Done | true/false values |
| Null Type | P0 | ✅ Done | null reference |
| Variables | P0 | ✅ Done | let declarations |
| Arithmetic Ops | P0 | ✅ Done | +, -, *, /, % |
| Comparison Ops | P0 | ✅ Done | ==, !=, <, >, <=, >= |
| Logical Ops | P0 | ⚠️ Partial | ! (done), &&, || (pending) |
| If/Else | P0 | ✅ Done | Conditional execution |
| Functions | P0 | ✅ Done | First-class functions |
| Closures | P0 | ✅ Done | Lexical scoping |
| Built-in Functions | P0 | ✅ Done | len, puts, first, last, rest, push |

### 4.2 Extended Features (v1.1)

| Feature | Priority | Status | Description |
|---------|----------|--------|-------------|
| While Loop | P1 | 📋 Planned | while (condition) { } |
| For Loop | P1 | 📋 Planned | for (init; cond; post) { } |
| Array Literals | P1 | 📋 Planned | [1, 2, 3] syntax |
| Hash/Map Literals | P1 | 📋 Planned | {key: value} syntax |
| Logical AND/OR | P1 | 📋 Planned | &&, || operators |
| Assignment Expression | P1 | 📋 Planned | x = value; without let |
| Compound Assignment | P2 | 📋 Planned | +=, -=, *=, /= |

### 4.3 Advanced Features (v1.2+)

| Feature | Priority | Status | Description |
|---------|----------|--------|-------------|
| Classes | P1 | 📋 Planned | class definition |
| Inheritance | P2 | 📋 Planned | extends keyword |
| Methods | P1 | 📋 Planned | class member functions |
| Constructors | P2 | 📋 Planned | init() method |
| Exception Handling | P2 | 📋 Planned | try/catch/throw |
| Modules | P2 | 📋 Planned | import/export |
| Standard Library | P2 | 📋 Planned | Math, String, Array modules |
| REPL History | P3 | 📋 Planned | Command history navigation |
| Debug Mode | P3 | 📋 Planned | Step-by-step execution |

---

## 5. Technical Requirements

### 5.1 Language Specification

#### 5.1.1 Lexical Elements

```
Keywords: let, const, fn, class, if, else, while, for, return, true, false, null

Operators: + - * / % = == != < <= > >= && || !

Delimiters: ( ) { } [ ] , ; .
```

#### 5.1.2 Grammar (EBNF)

```ebnf
program        = statement* ;

statement      = let_stmt | return_stmt | expr_stmt | block_stmt ;

let_stmt       = "let" identifier "=" expression ";" ;

return_stmt    = "return" expression? ";" ;

expr_stmt      = expression ";" ;

block_stmt     = "{" statement* "}" ;

expression     = assignment ;

assignment     = logic_or ( "=" assignment )? ;

logic_or       = logic_and ( "||" logic_and )* ;

logic_and      = equality ( "&&" equality )* ;

equality       = comparison ( ( "==" | "!=" ) comparison )* ;

comparison     = term ( ( "<" | "<=" | ">" | ">=" ) term )* ;

term           = factor ( ( "+" | "-" ) factor )* ;

factor         = unary ( ( "*" | "/" | "%" ) unary )* ;

unary          = ( "!" | "-" ) unary | call ;

call           = primary ( "(" arguments? ")" | "." identifier )* ;

primary        = INTEGER | FLOAT | STRING | "true" | "false" | "null"
               | identifier | "(" expression ")" ;
```

### 5.2 Performance Requirements

| Metric | Target | Notes |
|--------|--------|-------|
| Startup Time | < 10ms | Interpreter initialization |
| Memory Footprint | < 5MB | Base runtime |
| Function Call Overhead | < 1μs | Simple function call |

### 5.3 Portability

- **Compilers**: GCC 7+, Clang 5+, MSVC 2017+
- **Platforms**: Windows, macOS, Linux
- **Architecture**: x86_64, ARM64

---

## 6. API Design

### 6.1 C++ API

```cpp
#include "interpreter.h"

// Basic usage
script_lang::Interpreter interp;
interp.run("let x = 5; puts(x);");

// With environment
auto env = std::make_shared<script_lang::Environment>();
env->set("externalVar", script_lang::makeInteger(42));
interp.setEnvironment(env);

// Register native function
interp.registerFunction("nativeAdd", [](const std::vector<script_lang::ObjectPtr>& args) {
    // Implementation
});
```

### 6.2 REPL API

```cpp
script_lang::REPL repl;
repl.run(); // Interactive mode
```

---

## 7. Testing Strategy

### 7.1 Unit Tests

- Lexer tests: Token recognition
- Parser tests: AST generation
- Evaluator tests: Expression evaluation

### 7.2 Integration Tests

- Full program execution
- Error handling
- Edge cases

### 7.3 Coverage Target

- Line Coverage: > 80%
- Branch Coverage: > 70%

---

## 8. Documentation Requirements

| Document | Status | Description |
|----------|--------|-------------|
| README.md | ✅ Done | Project overview and quick start |
| CONTRIBUTING.md | ✅ Done | Development guidelines |
| ARCHITECTURE.md | 📋 Planned | System architecture |
| API.md | 📋 Planned | API reference |
| LANGUAGE.md | 📋 Planned | Language specification |

---

## 9. Release Milestones

### v1.0.0 (Current)

- [x] Basic data types
- [x] Variables and scoping
- [x] Arithmetic and comparison operators
- [x] If/else statements
- [x] Functions and closures
- [x] Built-in functions
- [x] Test suite
- [x] CMake build system
- [x] CI/CD pipeline

### v1.1.0 (Next)

- [ ] While/for loops
- [ ] Logical operators (&&, ||)
- [ ] Array literals
- [ ] Assignment expressions
- [ ] Error messages with line numbers

### v1.2.0 (Future)

- [ ] Class definitions
- [ ] Methods and constructors
- [ ] Exception handling
- [ ] Module system

---

## 10. Success Metrics

| Metric | Target | Measurement |
|--------|--------|-------------|
| GitHub Stars | 100+ | Repository popularity |
| Test Coverage | 80%+ | Code coverage tools |
| Build Success | 100% | CI/CD pipeline |
| Documentation | Complete | All docs written |
| Community | Active | Issues, PRs, discussions |

---

## 11. Risks and Mitigations

| Risk | Impact | Mitigation |
|------|--------|------------|
| Performance issues | Medium | Profile and optimize hot paths |
| Memory leaks | High | Use smart pointers, valgrind testing |
| Grammar complexity | Medium | Incremental feature additions |
| API breaking changes | High | Semantic versioning, deprecation policy |

---

## 12. References

- [Writing An Interpreter in Go](https://interpreterbook.com/) by Thorsten Ball
- [Crafting Interpreters](https://craftinginterpreters.com/) by Robert Nystrom
- [C++ Reference](https://en.cppreference.com/)
- [CMake Documentation](https://cmake.org/documentation/)