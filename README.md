# Infix Expression Converter and Evaluator

A robust C++ command-line application designed to parse arithmetic expressions written in infix notation, convert them into postfix notation (Reverse Polish Notation), and evaluate the final result dynamically based on user-defined variable values.

---

## Features

- **Tokenization & Parsing:** Efficiently decomposes complex input strings into individual tokens (operators, variables, integers, and grouping symbols).
- **Comprehensive Bracket Handling:** Full support for multiple grouping symbols including parentheses `()`, square brackets `[]`, and curly braces `{}`.
- **Operator Precedence:** Strictly adheres to standard arithmetic precedence rules (`*`, `/` take priority over `+`, `-`).
- **Dynamic Variable Acquisition:** Automatically identifies C++ compliant variable names within the expression and prompts the user for their numeric values at runtime.
- **Strict Stream Separation:** Detailed prompts and error messages are routed to `stderr`, while the final clean evaluation result is output directly to `stdout` to facilitate automated testing.

---

## Error Handling & Exit Codes

The system incorporates rigorous error-checking protocols to validate syntax and logical consistency, terminating with the following standard exit codes:

| Exit Code | Classification | Description |
| :---: | :--- | :--- |
| **0** | Success | Expression parsed, converted, and evaluated successfully. |
| **1** | Syntax Error | Invalid expression syntax or malformed tokens. |
| **2** | Runtime Error | Anomalies encountered during evaluation (e.g., division by zero). |
| **3** | Logical Error | Semantic or logical inconsistencies within the parsed expression. |

---

## Getting Started

### Prerequisites

To compile and run this project, you need a C++ compiler supporting at least C++11 or later (such as `g++`).

### Compilation

Clone the repository and compile the source code using the following terminal command:

```bash
g++ -std=c++11 q1.cpp -o expression_evaluator
