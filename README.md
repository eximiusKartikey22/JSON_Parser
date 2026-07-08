# Custom JSON Parser and Schema Validator

## Overview
- This project is a zero-dependency, lightweight JSON parser and serializer built entirely from scratch in modern C++. Rather than relying on external libraries, this engine safely deserializes raw text streams into a strongly-typed Abstract Syntax Tree (AST).
- The architecture focuses heavily on core systems engineering concepts, including lexical analysis, recursive descent parsing, dynamic memory management.

## Features & Implementation Details
- **Custom Lexical Analyzer (Tokenizer)**: Scans raw string inputs character-by-character and converts them into a clean, categorized token stream while safely handling whitespace and syntax anomalies.
- **Recursive Descent Parser**: Efficiently maps the token stream into an infinitely nestable Abstract Syntax Tree (AST), operating in highly optimal `O(N)` time complexity.
- **Strict Schema Validation**: Implements a validation engine that cross-references incoming JSON payloads against developer-defined templates to proactively catch missing keys, null values, or type mismatches.
- **Two-Way Serialization**: Dynamically converts in-memory C++ objects (like `std::map` and `std::vector`) back into valid JSON strings. It fully supports both minified formats for network transport and pretty-print formatting for human readability.
- **Dynamic Type Handling**: Seamlessly bridges untyped JSON data into standard C++ containers and primitive types.

## System Architecture
The pipeline processes incoming data in three distinct execution phases:

1. **Lexical Analysis:** Raw Text Stream `->` **[Lexer]** `->` Token Stream
2. **Tree Construction:** Token Stream `->` **[Parser]** `->` Abstract Syntax Tree (AST)
3. **Output & Verification:** AST `->` **[Serializer / Validator]** `->` Formatted JSON / Type-Safety Report

## Getting Started

### Prerequisites
**C++ Compiler:** Must support C++11 or higher (e.g., MSYS2 UCRT64 GCC on Windows, standard GCC/Clang on Linux/macOS, or MSVC).

### Installation & Execution

1. **Clone the Repository:**
   ```bash
   git clone [https://github.com/eximiusKartikey22/JSON-Parser.git](https://github.com/eximiusKartikey22/JSON-Parser.git)
   cd JSON-Parser
2. **Compile the Code:**

    ```bash
    g++ src/*.cpp -I include -o parser_app
    ```