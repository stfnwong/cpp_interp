# Crafting Interpreters (Nystrom)

This is a C++ implementation of the material in `Crafting Interpreters` by Robert 
Nystrom. I made this version because I got stuck on something in Rust and wanted to
have a reference to check against.

### Installation/Use
Executables are built into the root of the repo. To bulid the executables and tests 
run `make all`. Test binaries appear in the directory `/bin/test/`. A shell script is 
provided at `test/run_test.sh` to run all unit test binaries.


### Requirements 
- C++ 17
- I use `gcc`
- The standard library
- Tests use `Catch2`, header is in the project.


## Notes on Grammar
This is just a re-hash of the explanations from the book that I made to try and get the
concepts to stick. If you reading this to get a sense of the grammar for the language 
you are better off just reading the book instead.

## Syntax for Expressions
Expressions have a different rule for each precedence level.

Assignment has the lowest precedence (like in C).

`expression -> assignment`;

`assignment -> IDENTIFIER "=" assignment | logic_or;`

`logic_or -> logic_and ( "or" logic_and )* ;`

`logic_and -> equality ( "and" equality )* ;`

`equality -> comparison ( ( "!=" | "==") comparison )* ;`

`comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )* ;`

`term -> factor ( ( "-" | "+" ) factor )* ;`

`factor -> unary ( ("/" | "*") unary )* ;`

`unary -> ("!" | "-") unary | call ;`

For function calls we create a rule that matches a primary expression followed by
zero or more function calls. This means that not having parenthesis parses a regular 
primary expression, but adding a parenthesis pair makes it into a function call with
an optional list of arguments inside.

`call -> primary ( "(" arguments? ")" )* ;`

The implication of the `call` rule is that there is another `arguments` rule.

`arguments -> expression ( "," expression )*;`

This rule requires at least one argument expression followed by zero or more other
expressions, each preceded by a common. Zero-argument calls are handled by the fact that
the `call` rule considers the `arguments` production itself to be optional.

`primary -> NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" | IDENTIFIER ;`

## Syntax for Variables 
Variables have additional productions.

`program -> declaration* EOF;`

`declaration -> fun_decl | var_decl | statement ;`

`statement -> expr_stmt | if_stmt | for_stmt | while_stmt | print_stmt | block ;`

`if_stmt -> "if" "(" expression ")" statement ("else" statement )? ;`

`for_stmt -> "for" "(" ( var_decl | expr_stmt | ";" ) expression? ";" expression? ";" statement;`

`while_stmt -> "while" "(" expression ")" statement ;`

A block is just a (possibly empty) series of statements surrounded by curly braces.

`block -> "{" declaration* "}" ;`

A variable declaration looks like

`var_decl -> "var" IDENTIFIER  ( "=" expression)? ";"`

A function declaration looks like 

`fun_decl -> "fun" function ;`

`function -> IDENTIFIER "(" parameters? ")" block ;`
