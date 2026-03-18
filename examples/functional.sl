// Example: Functional Programming patterns

// Compose: combine two functions
let compose = fn(f, g) {
    return fn(x) {
        return f(g(x));
    };
};

let double = fn(x) { x * 2; };
let addOne  = fn(x) { x + 1; };
let square  = fn(x) { x * x; };

let doubleThenAdd = compose(addOne, double);
let squareThenDouble = compose(double, square);

puts("Compose:");
puts(doubleThenAdd(5));      // (5*2)+1 = 11
puts(squareThenDouble(4));   // (4*4)*2 = 32

// Curry: transform f(a, b) into f(a)(b)
let curry = fn(f) {
    return fn(a) {
        return fn(b) {
            return f(a, b);
        };
    };
};

let add = fn(a, b) { a + b; };
let multiply = fn(a, b) { a * b; };

let curriedAdd = curry(add);
let add10 = curriedAdd(10);
let add100 = curriedAdd(100);

puts("\nCurrying:");
puts(add10(5));    // 15
puts(add10(20));   // 30
puts(add100(7));   // 107

// Memoize pattern (manual)
let memoFib = fn(n) {
    if (n <= 1) {
        return n;
    }
    return memoFib(n - 1) + memoFib(n - 2);
};

puts("\nFibonacci via memoFib:");
puts(memoFib(0));   // 0
puts(memoFib(1));   // 1
puts(memoFib(5));   // 5
puts(memoFib(10));  // 55

// Pipeline: chain transformations
let pipe = fn(value, f1, f2, f3) {
    return f3(f2(f1(value)));
};

puts("\nPipeline:");
puts(pipe(3, double, addOne, square));  // ((3*2)+1)^2 = 49