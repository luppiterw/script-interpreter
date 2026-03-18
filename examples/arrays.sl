// Example: Data structures using arrays and higher-order functions

// Basic array usage
let numbers = [1, 2, 3, 4, 5];
let fruits = ["apple", "banana", "cherry"];

puts("Arrays:");
puts(first(numbers));     // 1
puts(last(numbers));      // 5
puts(len(numbers));       // 5
puts(len(fruits));        // 3

// Push and rest
let extended = push(numbers, 6);
puts("\nAfter push(numbers, 6):");
puts(len(extended));   // 6
puts(last(extended));  // 6

puts("\nRest of numbers:");
let tail = rest(numbers);
puts(first(tail));    // 2
puts(len(tail));      // 4

// Recursive sum
let sum = fn(arr) {
    if (len(arr) == 0) {
        return 0;
    }
    return first(arr) + sum(rest(arr));
};

puts("\nSum:");
puts(sum([1, 2, 3, 4, 5]));      // 15
puts(sum([10, 20, 30]));          // 60
puts(sum([]));                    // 0

// Map: apply function to each element
let map = fn(arr, f) {
    if (len(arr) == 0) {
        return [];
    }
    let head = first(arr);
    let tail = rest(arr);
    return push(map(tail, f), f(head));
};

// Reverse a map result to maintain order
let mapOrdered = fn(arr, f) {
    let accumulate = fn(remaining, acc) {
        if (len(remaining) == 0) {
            return acc;
        }
        let h = first(remaining);
        let newAcc = push(acc, f(h));
        return accumulate(rest(remaining), newAcc);
    };
    return accumulate(arr, []);
};

let double = fn(x) { x * 2; };
let square = fn(x) { x * x; };

let doubled = mapOrdered([1, 2, 3, 4, 5], double);
puts("\nDoubled:");
puts(first(doubled));               // 2
puts(first(rest(doubled)));         // 4
puts(first(rest(rest(doubled))));   // 6

// Filter: keep elements satisfying predicate
let filter = fn(arr, pred) {
    let accumulate = fn(remaining, acc) {
        if (len(remaining) == 0) {
            return acc;
        }
        let h = first(remaining);
        if (pred(h)) {
            return accumulate(rest(remaining), push(acc, h));
        }
        return accumulate(rest(remaining), acc);
    };
    return accumulate(arr, []);
};

let isEven = fn(x) { x - (x / 2) * 2 == 0; };

let evens = filter([1, 2, 3, 4, 5, 6, 7, 8], isEven);
puts("\nEven numbers from [1..8]:");
puts(len(evens));           // 4
puts(first(evens));         // 2
puts(last(evens));          // 8

// Reduce: fold array into a single value
let reduce = fn(arr, initial, f) {
    if (len(arr) == 0) {
        return initial;
    }
    let h = first(arr);
    return reduce(rest(arr), f(initial, h), f);
};

let add = fn(a, b) { a + b; };
let mul = fn(a, b) { a * b; };

puts("\nReduce:");
puts(reduce([1, 2, 3, 4, 5], 0, add));   // 15
puts(reduce([1, 2, 3, 4, 5], 1, mul));   // 120

// Flatten one level deep
let contains = fn(arr, val) {
    if (len(arr) == 0) {
        return false;
    }
    if (first(arr) == val) {
        return true;
    }
    return contains(rest(arr), val);
};

puts("\nContains:");
puts(contains([1, 2, 3, 4, 5], 3));    // true
puts(contains([1, 2, 3, 4, 5], 6));    // false
puts(contains(["a", "b", "c"], "b"));  // true
