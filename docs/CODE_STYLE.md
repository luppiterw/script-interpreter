# Code Style Guide

This document defines the coding standards for the Script Interpreter project.

---

## General Principles

1. **Readability First** - Code should be self-documenting
2. **Consistency** - Follow existing patterns in the codebase
3. **Simplicity** - Prefer simple solutions over clever ones
4. **Safety** - Use modern C++ features to prevent bugs

---

## C++ Style

### Naming Conventions

| Type | Convention | Example |
|------|------------|---------|
| Classes | PascalCase | `class Lexer` |
| Functions | camelCase | `nextToken()` |
| Variables | camelCase | `currentToken` |
| Constants | UPPER_SNAKE_CASE | `MAX_TOKEN_LENGTH` |
| Enum values | PascalCase | `TokenType::Integer` |
| Namespaces | snake_case | `namespace script_lang` |
| Files | snake_case | `lexer.cpp` |
| Private members | trailing underscore | `position_` |

### Header Guards

Use `#ifndef` style guards:

```cpp
#ifndef FILE_NAME_H
#define FILE_NAME_H

// ... content ...

#endif // FILE_NAME_H
```

### Includes

Order includes as follows:

```cpp
// 1. Related header
#include "my_class.h"

// 2. Project headers
#include "other_class.h"

// 3. System headers
#include <string>
#include <vector>

// 4. Third-party headers
// (none currently)
```

### Using Declarations

Prefer explicit namespace prefixes in headers:

```cpp
// Good (header file)
std::string tokenTypeToString(TokenType type);

// Avoid (header file)
using namespace std;
```

Using declarations are acceptable in implementation files:

```cpp
// Acceptable (cpp file)
using namespace script_lang;
```

---

## Modern C++ Features

### Smart Pointers

Always prefer smart pointers over raw pointers:

```cpp
// Good
std::unique_ptr<Expression> expr;
std::shared_ptr<Object> obj;

// Avoid
Expression* expr;
Object* obj;
```

### Auto

Use `auto` when type is obvious or verbose:

```cpp
// Good - type is obvious
auto token = lexer.nextToken();

// Good - type is verbose
auto it = std::find(tokens.begin(), tokens.end(), target);

// Avoid - type is not obvious
auto x = getValue();  // What type is x?
```

### Range-based For Loops

```cpp
// Good
for (const auto& token : tokens) {
    process(token);
}

// Avoid
for (size_t i = 0; i < tokens.size(); ++i) {
    process(tokens[i]);
}
```

### Const Correctness

Always use `const` where appropriate:

```cpp
// Member functions that don't modify state
std::string inspect() const override;

// Parameters that shouldn't be modified
void process(const std::string& input);

// Local variables that shouldn't change
const size_t length = input.length();
```

### Override Keyword

Always use `override` for virtual function overrides:

```cpp
class Integer : public Object {
public:
    ObjectType type() const override { return ObjectType::INTEGER; }
    std::string inspect() const override { return std::to_string(value); }
};
```

---

## Code Organization

### Class Declaration Order

```cpp
class MyClass {
public:
    // 1. Constructors and destructor
    MyClass();
    explicit MyClass(int value);
    ~MyClass();

    // 2. Public methods
    void publicMethod();
    int getValue() const;

    // 3. Static methods
    static MyClass create();

protected:
    // 4. Protected members

private:
    // 5. Private methods
    void helperMethod();

    // 6. Private members
    int value_;
    std::string name_;
};
```

### Function Size

- Functions should be small and focused
- Aim for < 30 lines per function
- Extract helper functions for complex logic

### File Size

- Keep files focused on a single responsibility
- If a file exceeds 500 lines, consider splitting

---

## Formatting

### Braces

Use Allman style (opening brace on new line):

```cpp
// Good
if (condition)
{
    doSomething();
}
else
{
    doOther();
}

// Also acceptable (K&R style)
if (condition) {
    doSomething();
}
```

### Indentation

- Use 4 spaces for indentation
- Do not use tabs

### Line Length

- Maximum 100 characters per line
- Break long lines at logical points:

```cpp
auto result = someLongFunction(
    argument1,
    argument2,
    argument3
);
```

### Whitespace

```cpp
// Space after keywords
if (condition) { }
while (condition) { }
for (int i = 0; i < n; ++i) { }

// Space around operators
int x = a + b * c;

// No space inside parentheses
function(arg1, arg2);

// No space before semicolon
doSomething();
```

---

## Comments

### Self-Documenting Code

Prefer clear naming over comments:

```cpp
// Bad
// Check if token is a number
if (t == 1) { }

// Good
if (token.type == TokenType::INTEGER) { }
```

### When to Comment

- Public API documentation
- Non-obvious algorithms
- Workarounds and their reasons
- TODO notes

### TODO Comments

```cpp
// TODO(username): Description of what needs to be done
// FIXME: Description of the issue
// HACK: Why this is a workaround
```

---

## Error Handling

### Return Values

Prefer returning error objects over exceptions:

```cpp
// Good
ObjectPtr divide(int a, int b) {
    if (b == 0) {
        return makeError("division by zero");
    }
    return makeInteger(a / b);
}

// Avoid (in this codebase)
ObjectPtr divide(int a, int b) {
    if (b == 0) {
        throw std::runtime_error("division by zero");
    }
    return makeInteger(a / b);
}
```

### Null Checks

Always check for null before use:

```cpp
if (auto integer = std::dynamic_pointer_cast<Integer>(obj)) {
    // Use integer->value
} else {
    // Handle error
}
```

---

## Testing

### Test Naming

```cpp
// test_<component>_<scenario>_<expected>
void testLexer_IntegerToken_ReturnsCorrectType();
void testParser_InvalidSyntax_ReportsError();
```

### Test Structure

```cpp
void testFeature() {
    // Arrange
    Lexer lexer("input");
    
    // Act
    auto result = lexer.nextToken();
    
    // Assert
    assert(result.type == TokenType::INTEGER);
}
```

---

## Git Commit Messages

Follow Conventional Commits:

```
<type>(<scope>): <subject>

<body>

<footer>
```

### Types

| Type | When to Use |
|------|-------------|
| feat | New feature |
| fix | Bug fix |
| docs | Documentation |
| style | Formatting |
| refactor | Code restructuring |
| test | Adding tests |
| chore | Maintenance |

### Examples

```bash
feat: add while loop support
fix(parser): handle empty expressions
docs: update README installation steps
refactor(lexer): extract comment handling
test(evaluator): add division by zero tests
```

---

## Code Review Checklist

Before submitting a PR, ensure:

- [ ] Code compiles without warnings
- [ ] All tests pass
- [ ] Code follows style guide
- [ ] No unnecessary changes
- [ ] Comments are clear and necessary
- [ ] Error handling is appropriate
- [ ] No memory leaks (smart pointers used)
- [ ] No undefined behavior

---

## Tools

### Recommended IDE Settings

- Format on save
- Show whitespace characters
- 100 character ruler
- Auto-add final newline

### Static Analysis

Consider using:

- clang-tidy
- cppcheck
- AddressSanitizer (ASan)
- UndefinedBehaviorSanitizer (UBSan)