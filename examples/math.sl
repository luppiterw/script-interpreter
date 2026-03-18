// Fibonacci sequence calculation
let fibonacci = fn(n) {
    if (n <= 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
};

puts("Fibonacci sequence:");
let i = 0;
while (i <= 10) {
    let fib = fibonacci(i);
    puts("fibonacci(" + i + ") = " + fib);
    i = i + 1;
}

// Factorial calculation
let factorial = fn(n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
};

puts("\nFactorial examples:");
puts("5! = " + factorial(5));
puts("7! = " + factorial(7));

// Prime number checker
let isPrime = fn(n) {
    if (n <= 1) {
        return false;
    }
    if (n <= 3) {
        return true;
    }
    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }
    
    let i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
        i = i + 6;
    }
    return true;
};

puts("\nPrime numbers up to 30:");
let num = 2;
while (num <= 30) {
    if (isPrime(num)) {
        puts(num);
    }
    num = num + 1;
}