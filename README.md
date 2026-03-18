# Script Language Interpreter

A complete scripting language interpreter implemented in C++17 with support for:
- Basic data types (integers, floats, strings, booleans)
- Control structures (if/else, while loops)
- Functions with closures
- Built-in functions
- REPL interface

## Features

### Language Features
- **Data Types**: Integer, Float, String, Boolean, Null
- **Variables**: `let` and `const` declarations
- **Control Flow**: If/else statements
- **Functions**: First-class functions with lexical scoping
- **Built-ins**: `len`, `puts`, `first`, `last`, `rest`, `push`
- **Operators**: Arithmetic (+, -, *, /, %), Comparison (==, !=, <, >, <=, >=), Logical (&&, ||, !)

### Technical Features
- Hand-written lexer and recursive descent parser
- Abstract Syntax Tree (AST) representation
- Tree-walk interpreter
- Environment-based scoping
- Error handling and reporting

## Building

### Prerequisites
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.14+

### Build Instructions

```bash
# Clone the repository
git clone <repository-url>
cd script_interpreter

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
cmake --build .

# Run tests
ctest
```

## Usage

### REPL Mode
```bash
./script_interpreter
```

This starts an interactive REPL where you can enter expressions:
```
Script Language REPL
Press Ctrl+C to exit

>>> let x = 5;
>>> let y = 10;
>>> x + y
15
>>> fn(x, y) { x * y }(3, 4)
12
```

### Script Mode
```bash
./script_interpreter script.sl
```

Create a script file (e.g., `example.sl`):
```javascript
let factorial = fn(n) {
    if (n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
};

let result = factorial(5);
puts(result);  // Output: 120
```

Then run it:
```bash
./script_interpreter example.sl
```

## Language Syntax

### Variables
```javascript
let x = 5;
let name = "Hello World";
let isTrue = true;
```

### Functions
```javascript
let add = fn(a, b) { return a + b; };
let result = add(3, 4);  // 7

// Anonymous functions
let multiply = fn(x, y) { x * y };
```

### Control Flow
```javascript
let max = fn(a, b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
};

// While loops
let counter = 0;
while (counter < 5) {
    puts(counter);
    counter = counter + 1;
}
```

### Built-in Functions
```javascript
// String length
len("hello");  // 5

// Array operations
let arr = [1, 2, 3, 4, 5];
len(arr);      // 5
first(arr);    // 1
last(arr);     // 5
rest(arr);     // [2, 3, 4, 5]
push(arr, 6);  // [1, 2, 3, 4, 5, 6]

// Output
puts("Hello, World!");
puts(42);
puts(true);
```

## Examples

### Fibonacci Sequence
```javascript
let fibonacci = fn(n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
};

let result = fibonacci(10);
puts(result);  // Output: 55
```

### Higher-order Functions
```javascript
let map = fn(arr, fn) {
    let result = [];
    let i = 0;
    while (i < len(arr)) {
        result = push(result, fn(arr[i]));
        i = i + 1;
    }
    return result;
};

let numbers = [1, 2, 3, 4, 5];
let doubled = map(numbers, fn(x) { x * 2 });
puts(doubled);  // Output: [2, 4, 6, 8, 10]
```

## Architecture

The interpreter follows a classic three-stage pipeline:

1. **Lexical Analysis**: `Lexer` converts source code into tokens
2. **Syntactic Analysis**: `Parser` builds an Abstract Syntax Tree (AST)
3. **Evaluation**: `Evaluator` walks the AST and executes the code

### Key Components

- **`token.h/cpp`**: Token definitions and keyword lookup
- **`lexer.h/cpp`**: Lexical analyzer
- **`parser.h/cpp`**: Recursive descent parser
- **`ast.h`**: Abstract Syntax Tree node definitions
- **`object.h`**: Runtime object system
- **`environment.h`**: Variable scoping and binding
- **`evaluator.h/cpp`**: Expression evaluator
- **`main.cpp`**: Main entry point and REPL

## Testing

The project includes comprehensive unit tests:

```bash
# Run all tests
ctest

# Run individual test suites
./lexer_test
./parser_test
./evaluator_test
```

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

This interpreter is inspired by the book "Writing An Interpreter In Go" by Thorsten Ball, adapted for C++ with additional features and modern C++ practices.