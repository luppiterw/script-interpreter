let name = "Script Language";
let version = 1.0;
let isAwesome = true;

puts("Welcome to " + name);

let a = 10;
let b = 20;
let sum = a + b;
let product = a * b;

puts("Sum: ");
puts(sum);
puts("Product: ");
puts(product);

let greet = fn(name) {
    return "Hello, " + name + "!";
};

let message = greet("Developer");
puts(message);

let score = 85;

if (score >= 90) {
    puts("Grade: A");
} else {
    if (score >= 80) {
        puts("Grade: B");
    } else {
        if (score >= 70) {
            puts("Grade: C");
        } else {
            puts("Grade: F");
        }
    }
}

puts("Counting:");
puts(0);
puts(1);
puts(2);
puts(3);
puts(4);
puts(5);