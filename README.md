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

`statement -> expr_stmt | if_stmt | for_stmt | return_stmt | while_stmt | print_stmt | block ;`

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

Values returned from a function have their own statement

`return_stmt -> "return" expression? ";" ;`



## Design notes 
I templated the Visitor classes which are parameterized over `E` (the "expression" type)
and `V` (the "visit" type). In theory means that visitors can hold and return arbitrary
types. In practice you don't really want to do this. In fact it turns out to be quite 
awkward to do this because

- You have to keep specifiying the types everywhere since they are templates. That is, you can't just have a `Stmt` or an `Expr`, you need a `Stmt<E, V>` or an `Expr<E, V>`.
- Ok fine, big deal. In practice this sucks because there is only really one type that you actually want for each of these. For example, you always want to return a `LoxObject` (or perhaps a pointer to a `LoxObject`) in every visit method.
- This requirement to return a `LoxObject` makes things doubly stupid for the `ASTVisitor`, since that also has to be parameterized over `E` and `V`.
- This in turn means that you can only visit an `Expr<E, V>` or `Stmt<E, V>`.
- The interpreter wants to have `Expr<LoxObject, LoxObject>` and `Stmt<LoxObject, LoxObject>`.
- It would be nice to have `Expr<LoxObject, std::string>` in the `ASTPrinter`, since that would return a `std::string` directly, but you can't do this since you can't visit a `Expr<LoxObject, LoxObject>` for example, which is what the interpreter actually uses. 


At the time of writing I am leaving this templated implementation in, but I think its
better to re-write it to have fixed types. There doesn't seem to be much use for being
able to parameterize the visitor classes over multiple types anyway, and as such all
you get for doing so is downsides.

