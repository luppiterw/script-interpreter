# Questions & Development Notes

This document tracks open questions, design decisions, and TODO items for future development.

---

## Open Questions

### Architecture Decisions

#### Q1: Should we use unique_ptr or shared_ptr for AST nodes?

**Status:** Decided  
**Decision:** Use `unique_ptr` for AST nodes owned by parent nodes.  
**Rationale:** 
- Clear ownership semantics
- Better performance (no reference counting overhead)
- Prevents accidental shared ownership

**Note:** The Function object needs `shared_ptr<BlockStatement>` because functions can be passed around and need to reference their body. This requires careful handling with non-owning shared_ptr.

---

#### Q2: How should we handle memory management for closures?

**Status:** Open  
**Options:**
1. Capture by value (copy variables into closure)
2. Capture by reference (shared_ptr to environment)
3. Mixed approach with explicit capture list

**Current Implementation:** Shared environment pointer

**Concerns:**
- Potential memory leaks with circular references
- Performance impact of shared_ptr

---

#### Q3: Should we implement a bytecode VM or continue with tree-walking?

**Status:** Open  
**Current:** Tree-walking interpreter

**Pros of Bytecode VM:**
- Better performance (10-100x faster)
- Easier optimization
- Can implement JIT later

**Pros of Tree-walking:**
- Simpler implementation
- Easier debugging
- Faster startup (no compilation phase)

**Decision:** Keep tree-walking for v1.x, consider bytecode for v2.0

---

### Language Design Questions

#### Q4: Should we support semicolon-less syntax?

**Status:** Decided  
**Decision:** Require semicolons for now  
**Rationale:**
- Simpler parser
- Clearer statement boundaries
- Familiar to C/C++/JavaScript developers

---

#### Q5: How should we handle null/nil?

**Status:** Decided  
**Decision:** Use `null` keyword  
**Rationale:**
- Consistent with JavaScript, Java
- Clear semantic meaning
- Easy to implement

---

#### Q6: Should we support variadic functions?

**Status:** Open  
**Options:**
1. No variadic support (simpler)
2. `...args` syntax like JavaScript
3. Function overloading

**Consideration:** Built-in functions already support variadic args via vector

---

#### Q7: How should we implement string interpolation?

**Status:** Open  
**Proposals:**
```javascript
// Option 1: Template literals (JavaScript-like)
let name = "World";
let greeting = `Hello, ${name}!`;

// Option 2: Format function
let greeting = format("Hello, {}!", name);

// Option 3: String concatenation (current)
let greeting = "Hello, " + name + "!";
```

---

#### Q8: Should we support pattern matching?

**Status:** Open (Future)  
**Example:**
```javascript
match (value) {
    case 0: "zero";
    case 1: "one";
    case _: "other";
}
```

---

## Design Decisions Log

| Date | Decision | Rationale |
|------|----------|-----------|
| 2026-03-18 | Use `fn` keyword for functions | Shorter than `function`, familiar from Rust |
| 2026-03-18 | Single-pass lexer | Simpler implementation |
| 2026-03-18 | Recursive descent parser | Easier to extend than parser generators |
| 2026-03-18 | Truthy values for if conditions | More flexible, JavaScript-like |
| 2026-03-18 | `let` for variable declaration | Familiar, indicates mutable binding |

---

## TODO List

### High Priority (v1.1)

- [ ] **Implement `while` loop**
  - Add WHILE token
  - Parse while statement
  - Evaluate loop with condition check
  - Handle break/continue (optional)

- [ ] **Implement `for` loop**
  - Syntax: `for (init; condition; post) { body }`
  - Add FOR token
  - Handle scope correctly

- [ ] **Implement `&&` and `||` operators**
  - Add to lexer
  - Add to parser precedence
  - Short-circuit evaluation

- [ ] **Add array literal syntax**
  - Parse `[expr, expr, ...]`
  - Create Array object
  - Support empty arrays `[]`

- [ ] **Improve error messages**
  - Include line and column numbers
  - Show context around error
  - Suggest fixes for common mistakes

### Medium Priority (v1.2)

- [ ] **Implement class definitions**
  - `class Name { methods }`
  - Constructor handling
  - `this` keyword
  - Instance field access

- [ ] **Add assignment expressions**
  - `x = value;` without `let`
  - Return assigned value

- [ ] **Add compound assignment operators**
  - `+=`, `-=`, `*=`, `/=`, `%=`

- [ ] **Implement increment/decrement**
  - `++`, `--` operators
  - Prefix and postfix forms

- [ ] **Add switch statement**
  - `switch (expr) { case ... }`
  - Fall-through behavior

### Low Priority (v2.0)

- [ ] **Implement module system**
  - `import` statement
  - `export` statement
  - File-based modules

- [ ] **Add exception handling**
  - `try/catch/throw`
  - Error types

- [ ] **Implement generators**
  - `yield` keyword
  - Iterator protocol

- [ ] **Add async/await**
  - Promise-like objects
  - Non-blocking execution

---

## Known Issues

### Issue 1: Function body shared_ptr hack

**Description:** The Function object uses a non-owning `shared_ptr` to reference the AST body. This works but is not ideal.

**Location:** `src/evaluator.cpp:168`

**Workaround:** The AST must outlive the function execution.

**Proper Fix:** Consider copying the body or using a different ownership model.

---

### Issue 2: No line numbers in error messages

**Description:** Runtime errors don't show the source location.

**Example:**
```
ERROR: unknown operator: 85 >= 90
```

**Expected:**
```
ERROR at line 15, column 5: unknown operator: 85 >= 90
```

**Fix:** Store token positions in AST nodes and propagate to runtime errors.

---

### Issue 3: No string escape sequences

**Description:** Strings don't support `\n`, `\t`, `\"`, etc.

**Example:**
```javascript
let s = "Hello\nWorld";  // \n is literal backslash-n, not newline
```

**Fix:** Implement escape sequence handling in `readString()`.

---

## Performance Considerations

### Memory

- [ ] Profile memory usage with large scripts
- [ ] Consider object pooling for frequently created objects
- [ ] Implement garbage collection? (for closures)

### Speed

- [ ] Benchmark function call overhead
- [ ] Consider string interning for identifiers
- [ ] Optimize hot paths in evaluator

---

## Compatibility Notes

### C++17 Features Used

- `std::variant` (not currently used, planned)
- `std::optional` (not currently used, planned)
- `std::string_view` (can optimize string handling)
- Structured bindings (can simplify code)
- `if constexpr` (for template metaprogramming)

### Compiler-Specific

- MSVC: `/std:c++17`
- GCC: `-std=c++17`
- Clang: `-std=c++17`

---

## References

- [ECMAScript Specification](https://tc39.es/ecma262/)
- [Lua 5.4 Reference Manual](https://www.lua.org/manual/5.4/)
- [Python Language Reference](https://docs.python.org/3/reference/)
- [Rust Reference](https://doc.rust-lang.org/reference/)