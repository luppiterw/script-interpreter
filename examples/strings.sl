// Example: String operations and manipulation

// Basic string operations
let greeting = "Hello";
let name = "World";
let message = greeting + ", " + name + "!";

puts("Basic strings:");
puts(message);                      // Hello, World!
puts(len(greeting));                // 5
puts(len(""));                      // 0
puts(len("ScriptLang"));           // 10

// String comparison
let s1 = "apple";
let s2 = "banana";
let s3 = "apple";

puts("\nString comparison:");
puts(s1 == s3);    // true
puts(s1 == s2);    // false
puts(s1 != s2);    // true

// Building strings
let repeat = fn(str, times) {
    if (times <= 0) {
        return "";
    }
    return str + repeat(str, times - 1);
};

puts("\nRepeat:");
puts(repeat("ab", 3));    // ababab
puts(repeat("*", 5));     // *****
puts(repeat("Hi!", 2));   // Hi!Hi!

// String as function argument
let greet = fn(first, last) {
    return "Hello, " + first + " " + last + "!";
};

puts("\nGreeting function:");
puts(greet("John", "Doe"));    // Hello, John Doe!
puts(greet("Jane", "Smith"));  // Hello, Jane Smith!

// Checking string length conditions
let classify = fn(str) {
    let l = len(str);
    if (l == 0) {
        return "empty";
    }
    if (l < 5) {
        return "short";
    }
    if (l < 10) {
        return "medium";
    }
    return "long";
};

puts("\nString classification:");
puts(classify(""));           // empty
puts(classify("hi"));         // short
puts(classify("hello"));      // short
puts(classify("greetings"));  // medium
puts(classify("superlongstringhere")); // long
