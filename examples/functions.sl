// Higher-order functions example

// Map function - applies a function to each element of an array
let map = fn(arr, transformFn) {
    let result = [];
    let i = 0;
    while (i < len(arr)) {
        result = push(result, transformFn(arr[i]));
        i = i + 1;
    }
    return result;
};

// Filter function - filters array elements based on predicate
let filter = fn(arr, predicateFn) {
    let result = [];
    let i = 0;
    while (i < len(arr)) {
        if (predicateFn(arr[i])) {
            result = push(result, arr[i]);
        }
        i = i + 1;
    }
    return result;
};

// Reduce function - reduces array to single value
let reduce = fn(arr, reducerFn, initialValue) {
    let accumulator = initialValue;
    let i = 0;
    while (i < len(arr)) {
        accumulator = reducerFn(accumulator, arr[i]);
        i = i + 1;
    }
    return accumulator;
};

// Test data
let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];

puts("Original array: " + numbers);

// Double each number
let doubled = map(numbers, fn(x) { x * 2 });
puts("Doubled: " + doubled);

// Square each number
let squared = map(numbers, fn(x) { x * x });
puts("Squared: " + squared);

// Filter even numbers
let evens = filter(numbers, fn(x) { x % 2 == 0 });
puts("Even numbers: " + evens);

// Filter odd numbers
let odds = filter(numbers, fn(x) { x % 2 != 0 });
puts("Odd numbers: " + odds);

// Sum all numbers
let sum = reduce(numbers, fn(acc, x) { acc + x }, 0);
puts("Sum: " + sum);

// Product of all numbers
let product = reduce(numbers, fn(acc, x) { acc * x }, 1);
puts("Product: " + product);

// Find maximum
let max = reduce(numbers, fn(acc, x) { 
    if (x > acc) { 
        return x; 
    } else { 
        return acc; 
    } 
}, numbers[0]);
puts("Maximum: " + max);

// String processing example
let words = ["hello", "world", "script", "language"];

let uppercased = map(words, fn(word) {
    // Simple uppercase conversion (first letter only for demo)
    return word;  // In a real implementation, you'd convert case
});

puts("Words: " + words);
puts("Processed: " + uppercased);