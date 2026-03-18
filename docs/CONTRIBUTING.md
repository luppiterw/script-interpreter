# Contributing Guide

Thank you for your interest in contributing to the Script Interpreter project!

## Development Standards

### Commit Message Convention

We follow [Conventional Commits](https://www.conventionalcommits.org/) specification for all commit messages.

#### Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

#### Types

| Type | Description |
|------|-------------|
| `feat` | A new feature |
| `fix` | A bug fix |
| `docs` | Documentation only changes |
| `style` | Changes that do not affect the meaning of the code (white-space, formatting, missing semi-colons, etc) |
| `refactor` | A code change that neither fixes a bug nor adds a feature |
| `perf` | A code change that improves performance |
| `test` | Adding missing tests or correcting existing tests |
| `build` | Changes that affect the build system or external dependencies |
| `ci` | Changes to CI configuration files and scripts |
| `chore` | Other changes that don't modify src or test files |
| `revert` | Reverts a previous commit |

#### Examples

```bash
# Feature
feat: add while loop support

# Feature with scope
feat(parser): add array literal parsing

# Bug fix
fix: handle division by zero in evaluator

# Documentation
docs: update README with installation instructions

# Breaking change
feat!: change function syntax from 'fn' to 'function'

BREAKING CHANGE: The 'fn' keyword is no longer supported.
Use 'function' keyword instead.
```

### Branch Naming Convention

| Branch Type | Format | Example |
|-------------|--------|---------|
| Feature | `feat/<description>` | `feat/while-loop` |
| Bug Fix | `fix/<description>` | `fix/division-by-zero` |
| Documentation | `docs/<description>` | `docs/api-reference` |
| Refactor | `refactor/<description>` | `refactor/lexer-performance` |

### Pull Request Process

1. Fork the repository
2. Create a feature branch from `main`
3. Make your changes following code style guidelines
4. Add/update tests for your changes
5. Ensure all tests pass: `ctest`
6. Update documentation if needed
7. Create a Pull Request with a clear description

### Code Review Checklist

- [ ] Code follows the style guidelines
- [ ] Self-review of the code has been performed
- [ ] Comments are added for complex logic
- [ ] Documentation is updated
- [ ] No new warnings are introduced
- [ ] Tests are added/updated
- [ ] All tests pass

## Development Setup

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.14+

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/luppiterw/script-interpreter.git
cd script-interpreter

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build .

# Run tests
ctest --output-on-failure
```

### Project Structure

```
script_interpreter/
├── include/          # Header files
├── src/              # Source files
├── tests/            # Unit tests
├── examples/         # Example scripts
├── docs/             # Documentation
└── .github/          # GitHub workflows
```

## Questions?

Feel free to open an issue for any questions or suggestions!