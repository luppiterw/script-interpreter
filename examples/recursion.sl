// Example: Recursive algorithms
// Demonstrates recursion patterns: linear, tree, mutual recursion

// ---- 1. Factorial ----
let factorial = fn(n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
};

puts("Factorials:");
puts(factorial(0));   // 1
puts(factorial(1));   // 1
puts(factorial(5));   // 120
puts(factorial(10));  // 3628800

// ---- 2. Fibonacci ----
let fib = fn(n) {
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
};

puts("\nFibonacci:");
puts(fib(0));   // 0
puts(fib(1));   // 1
puts(fib(7));   // 13
puts(fib(10));  // 55

// ---- 3. Fibonacci with memoization via accumulators ----
let fibIter = fn(n) {
    let go = fn(n, a, b) {
        if (n == 0) { return a; }
        return go(n - 1, b, a + b);
    };
    return go(n, 0, 1);
};

puts("\nFibonacci (iterative style):");
puts(fibIter(0));   // 0
puts(fibIter(1));   // 1
puts(fibIter(10));  // 55
puts(fibIter(20));  // 6765

// ---- 4. Countdown ----
let countdown = fn(n) {
    if (n <= 0) {
        puts("Go!");
        return 0;
    }
    puts(n);
    return countdown(n - 1);
};

puts("\nCountdown:");
countdown(5);

// ---- 5. Sum of range ----
let sumRange = fn(from, to) {
    if (from > to) {
        return 0;
    }
    return from + sumRange(from + 1, to);
};

puts("\nSum of range:");
puts(sumRange(1, 10));   // 55
puts(sumRange(1, 100));  // 5050

// ---- 6. Power (exponentiation by squaring) ----
let pow = fn(base, exp) {
    if (exp == 0) { return 1; }
    if (exp == 1) { return base; }
    let half = pow(base, exp / 2);
    if ((exp - (exp / 2) * 2) == 0) {
        return half * half;
    }
    return base * half * half;
};

puts("\nPower:");
puts(pow(2, 10));   // 1024
puts(pow(3, 5));    // 243
puts(pow(5, 3));    // 125

// ---- 7. Integer to string (base 10) ----
let intToStr = fn(n) {
    let digits = "0123456789";
    if (n < 0) {
        return "-" + intToStr(-n);
    }
    if (n < 10) {
        // Use a lookup approach via recursion
        let digitChar = fn(d) {
            if (d == 0) { return "0"; }
            if (d == 1) { return "1"; }
            if (d == 2) { return "2"; }
            if (d == 3) { return "3"; }
            if (d == 4) { return "4"; }
            if (d == 5) { return "5"; }
            if (d == 6) { return "6"; }
            if (d == 7) { return "7"; }
            if (d == 8) { return "8"; }
            return "9";
        };
        return digitChar(n);
    }
    let lastDigit = n - (n / 10) * 10;
    let rest = n / 10;
    let digitChar = fn(d) {
        if (d == 0) { return "0"; }
        if (d == 1) { return "1"; }
        if (d == 2) { return "2"; }
        if (d == 3) { return "3"; }
        if (d == 4) { return "4"; }
        if (d == 5) { return "5"; }
        if (d == 6) { return "6"; }
        if (d == 7) { return "7"; }
        if (d == 8) { return "8"; }
        return "9";
    };
    return intToStr(rest) + digitChar(lastDigit);
};

puts("\nInteger to string:");
puts(intToStr(0));       // 0
puts(intToStr(42));      // 42
puts(intToStr(1024));    // 1024
puts(intToStr(-99));     // -99
