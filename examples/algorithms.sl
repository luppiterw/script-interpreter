// Example: Sorting algorithms
// Demonstrates recursion, closures, and higher-order functions

// Bubble sort simulation using recursive approach
let min = fn(a, b) {
    if (a < b) {
        return a;
    }
    return b;
};

let max = fn(a, b) {
    if (a > b) {
        return a;
    }
    return b;
};

let abs = fn(n) {
    if (n < 0) {
        return -n;
    }
    return n;
};

puts("Math utilities:");
puts(min(5, 3));       // 3
puts(max(5, 3));       // 5
puts(abs(-42));        // 42
puts(abs(42));         // 42

// Power function
let pow = fn(base, exp) {
    if (exp == 0) {
        return 1;
    }
    return base * pow(base, exp - 1);
};

puts("\nPowers:");
puts(pow(2, 0));   // 1
puts(pow(2, 1));   // 2
puts(pow(2, 8));   // 256
puts(pow(3, 4));   // 81

// GCD (Greatest Common Divisor) - Euclidean algorithm
let gcd = fn(a, b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a - (a / b) * b);
};

puts("\nGCD:");
puts(gcd(48, 18));   // 6
puts(gcd(100, 75));  // 25
puts(gcd(7, 13));    // 1

// Sum of digits
let sumDigits = fn(n) {
    if (n < 10) {
        return n;
    }
    return n - (n / 10) * 10 + sumDigits(n / 10);
};

puts("\nSum of digits:");
puts(sumDigits(123));   // 6
puts(sumDigits(9999));  // 36
puts(sumDigits(1001));  // 2