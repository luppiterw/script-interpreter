# Architecture Overview

This document describes the architecture and design of the Script Interpreter.

---

## System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                         Source Code                              │
└─────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                          Lexer                                   │
│  ┌─────────────────────────────────────────────────────────┐    │
│  │  Input: "let x = 5 + 3;"                                │    │
│  │  Output: [LET, IDENTIFIER("x"), ASSIGN,                 │    │
│  │           INTEGER(5), PLUS, INTEGER(3), SEMICOLON, EOF] │    │
│  └─────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                          Parser                                  │
│  ┌─────────────────────────────────────────────────────────┐    │
│  │  Input: Token stream                                    │    │
│  │  Output: Abstract Syntax Tree (AST)                     │    │
│  │                                                          │    │
│  │         Program                                         │    │
│  │           │                                              │    │
│  │         LetStatement                                    │    │
│  │         /      \                                        │    │
│  │    Identifier  InfixExpression                          │    │
│  │       "x"       /        \                              │    │
│  │            Integer    Integer                           │    │
│  │               5          3                              │    │
│  └─────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                         Evaluator                                │
│  ┌─────────────────────────────────────────────────────────┐    │
│  │  Input: AST                                             │    │
│  │  Output: Runtime Objects                                │    │
│  │                                                          │    │
│  │  1. Walk the AST                                        │    │
│  │  2. Evaluate expressions                                │    │
│  │  3. Execute statements                                  │    │
│  │  4. Manage environment/scope                            │    │
│  └─────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────┘
                                │
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                       Runtime Objects                            │
│  ┌───────────┐ ┌───────────┐ ┌───────────┐ ┌───────────┐        │
│  │  Integer  │ │   Float   │ │  String   │ │  Boolean  │        │
│  └───────────┘ └───────────┘ └───────────┘ └───────────┘        │
│  ┌───────────┐ ┌───────────┐ ┌───────────┐ ┌───────────┐        │
│  │   Null    │ │  Function │ │  Builtin  │ │   Array   │        │
│  └───────────┘ └───────────┘ └───────────┘ └───────────┘        │
└─────────────────────────────────────────────────────────────────┘
```

---

## Component Details

### 1. Lexer (`lexer.h/cpp`)

**Responsibility:** Convert source code into tokens.

**Key Components:**

| Component | Description |
|-----------|-------------|
| `Lexer` class | Main lexer implementation |
| `nextToken()` | Returns the next token |
| `readChar()` | Advances position |
| `skipWhitespace()` | Skips whitespace and comments |

**Token Types:**

```cpp
enum class TokenType {
    // Literals
    INTEGER, FLOAT, STRING, BOOLEAN, IDENTIFIER,
    
    // Keywords
    LET, CONST, FUNCTION, CLASS, IF, ELSE, WHILE, FOR, RETURN,
    
    // Operators
    PLUS, MINUS, MULTIPLY, DIVIDE, MODULO,
    ASSIGN, EQUAL, NOT_EQUAL, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL,
    AND, OR, NOT,
    
    // Delimiters
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET, COMMA, SEMICOLON, DOT,
    
    // Special
    EOF_, ILLEGAL
};
```

---

### 2. Parser (`parser.h/cpp`)

**Responsibility:** Convert tokens into an Abstract Syntax Tree (AST).

**Parser Type:** Recursive Descent with Pratt Parsing for expressions

**Operator Precedence (lowest to highest):**

```
LOWEST
  ├── EQUALS (==, !=)
  ├── LESSGREATER (<, >, <=, >=)
  ├── SUM (+, -)
  ├── PRODUCT (*, /, %)
  ├── PREFIX (!, -)
  └── CALL (function())
HIGHEST
```

**Key Methods:**

| Method | Description |
|--------|-------------|
| `parseProgram()` | Entry point, returns AST root |
| `parseStatement()` | Parses a single statement |
| `parseExpression()` | Parses expression with precedence |
| `parsePrefixExpression()` | Handles prefix operators |
| `parseInfixExpression()` | Handles infix operators |

---

### 3. AST (`ast.h`)

**Responsibility:** Define AST node types.

**Node Hierarchy:**

```
Node (abstract)
├── Statement (abstract)
│   ├── LetStatement
│   ├── ReturnStatement
│   ├── ExpressionStatement
│   └── BlockStatement
└── Expression (abstract)
    ├── Identifier
    ├── IntegerLiteral
    ├── FloatLiteral
    ├── StringLiteral
    ├── BooleanLiteral
    ├── PrefixExpression
    ├── InfixExpression
    ├── IfExpression
    ├── FunctionLiteral
    └── CallExpression
```

---

### 4. Object System (`object.h`)

**Responsibility:** Define runtime object types.

**Object Hierarchy:**

```
Object (abstract)
├── Integer
├── Float
├── String
├── Boolean
├── Null
├── ReturnValue (wrapper for return statements)
├── Error
├── Function
├── Builtin
├── Array
├── Hash
├── Class
└── Instance
```

**Memory Management:** Uses `std::shared_ptr<Object>` (aliased as `ObjectPtr`)

---

### 5. Environment (`environment.h`)

**Responsibility:** Manage variable bindings and scope.

**Features:**

- Chained scope (inner scope can access outer scope)
- Variable lookup with scope chain traversal
- Variable assignment in current scope

**Structure:**

```cpp
class Environment {
    std::unordered_map<std::string, ObjectPtr> store;
    std::shared_ptr<Environment> outer;  // Parent scope
};
```

**Scope Chain Example:**

```
Global Scope
    │
    └── Function Scope
            │
            └── Block Scope
```

---

### 6. Evaluator (`evaluator.h/cpp`)

**Responsibility:** Execute the AST and produce results.

**Key Methods:**

| Method | Description |
|--------|-------------|
| `evaluate()` | Entry point |
| `evalProgram()` | Evaluates all statements |
| `evalStatement()` | Evaluates a single statement |
| `evalExpression()` | Evaluates an expression |
| `evalInfixExpression()` | Handles binary operators |
| `evalIfExpression()` | Handles if/else |
| `applyFunction()` | Calls functions |

---

## Data Flow

### Example: `let x = 5 + 3;`

```
1. Lexer Phase:
   Source: "let x = 5 + 3;"
   Tokens: [LET, IDENTIFIER("x"), ASSIGN, INTEGER("5"), PLUS, INTEGER("3"), SEMICOLON, EOF]

2. Parser Phase:
   AST:
   Program
     └── LetStatement
             ├── Identifier("x")
             └── InfixExpression
                     ├── Integer(5)
                     ├── op: "+"
                     └── Integer(3)

3. Evaluator Phase:
   - Evaluate InfixExpression: 5 + 3 = 8
   - Create Integer(8)
   - Store in environment: env["x"] = Integer(8)
   - Return Integer(8)
```

---

## Error Handling

### Compile-time Errors (Parser)

```cpp
// Parser stores errors in a vector
std::vector<std::string> errors;

// Example error
"expected next token to be IDENTIFIER, got INTEGER instead at line 5, column 10"
```

### Runtime Errors (Evaluator)

```cpp
// Returns Error object
return makeError("division by zero");
return makeError("identifier not found: x");
return makeError("unknown operator: INTEGER + BOOLEAN");
```

---

## Extension Points

### Adding a New Statement Type

1. Add token type in `token.h`
2. Add AST node in `ast.h`
3. Add parsing method in `parser.cpp`
4. Add evaluation method in `evaluator.cpp`

### Adding a New Operator

1. Add token type in `token.h`
2. Add precedence in `parser.cpp`
3. Register in prefix/infix parse functions
4. Add evaluation logic in `evaluator.cpp`

### Adding a New Built-in Function

1. Add to `initBuiltins()` in `evaluator.cpp`
2. Document in README.md

---

## Testing Strategy

```
tests/
├── lexer_test.cpp      # Token generation
├── parser_test.cpp     # AST generation
└── evaluator_test.cpp  # Expression evaluation
```

**Test Coverage:**

| Component | Coverage Focus |
|-----------|---------------|
| Lexer | All token types, edge cases |
| Parser | All AST node types, precedence |
| Evaluator | All operations, error handling |

---

## Build System

```
CMakeLists.txt
├── Library: script_interpreter_lib
├── Executable: script_interpreter
└── Tests: lexer_test, parser_test, evaluator_test
```

**Dependencies:**

- C++17 standard library only
- No external dependencies for core functionality

---

## Future Improvements

### Short-term

- [ ] Add line/column info to AST nodes
- [ ] Improve error messages
- [ ] Add more built-in functions

### Medium-term

- [ ] Implement while/for loops
- [ ] Add class support
- [ ] Optimize hot paths

### Long-term

- [ ] Bytecode compilation
- [ ] JIT compilation
- [ ] Garbage collection