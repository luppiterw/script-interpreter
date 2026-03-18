// Example: Closures and Scope

// 1. Basic closure - captures outer variable
let makeGreeter = fn(greeting) {
    return fn(name) {
        return greeting + ", " + name + "!";
    };
};

let sayHello = makeGreeter("Hello");
let sayHi    = makeGreeter("Hi");
let sayHey   = makeGreeter("Hey");

puts("Closures:");
puts(sayHello("Alice"));   // Hello, Alice!
puts(sayHi("Bob"));        // Hi, Bob!
puts(sayHey("Charlie"));   // Hey, Charlie!

// 2. Accumulator closure
let makeAccumulator = fn(initial) {
    return fn(n) {
        return initial + n;
    };
};

let addFrom10  = makeAccumulator(10);
let addFrom100 = makeAccumulator(100);

puts("\nAccumulator:");
puts(addFrom10(5));    // 15
puts(addFrom10(20));   // 30
puts(addFrom100(42));  // 142

// 3. Private state via closure
let makeStack = fn() {
    return fn(operation, value) {
        if (operation == "size") {
            return 0;
        }
        return null;
    };
};

// 4. Adder factory
let makeMultiplier = fn(factor) {
    return fn(x) { factor * x; };
};

let double = makeMultiplier(2);
let triple = makeMultiplier(3);
let tenX   = makeMultiplier(10);

puts("\nMultipliers:");
puts(double(7));    // 14
puts(triple(7));    // 21
puts(tenX(7));      // 70

// 5. Nested closures
let outerScope = fn(a) {
    return fn(b) {
        return fn(c) {
            return a + b + c;
        };
    };
};

puts("\nNested closures:");
puts(outerScope(1)(2)(3));    // 6
puts(outerScope(10)(20)(30)); // 60