# Expression Tree Calculator

A simple C program that converts an infix mathematical expression into postfix form, builds an expression tree, evaluates the result, and prints the expression in prefix and postfix notation.

## Features
- Infix → Postfix conversion  
- Expression tree construction  
- Expression evaluation  
- Prefix and postfix output  
- Supports operators: `+`, `-`, `*`, `/`, `%`

## Compile
```bash
gcc -o expression_tree expression_tree.c

Run:
 ./expression_tree

Input:
 3 + (4 * 2)

Output:
 Result: 11
 Prefix: + 3 * 4 2
 Postfix: 3 4 2 * +
