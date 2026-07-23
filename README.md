# KalkCLI-ator
## Overview
KalkCLI-ator (or kalkcliator) is a calculator CLI used to evaluate expressions. This includes most of the functions in the `cmath` header.

## Usage
The program receives an argument which does different things depending on the argument.

### Arguments
#### Expression as argument
It will evalueate and print out the expression. For example,
```bash
username$ kalkcliator "1+sin(30deg)"
 = 1.5
```

##### Syntax and Grammar
Here are the grammar rules in Backus-Naur Form (BNF)
```
<parse>           ::= <expr> END_OF_FILE

<expr>            ::= <term> <expr_tail>
<expr_tail>       ::= "+" <term> <expr_tail>
                    | "-" <term> <expr_tail>
                    | ε

<term>            ::= <power> <term_tail>
<term_tail>       ::= "*" <power> <term_tail>
                    | "/" <power> <term_tail>
                    | <implicit_mul_power> <term_tail>
                    | ε

<power>           ::= <factor> "^" <power>
                    | <factor>

<factor>          ::= <number_literal>
                    | "(" <expr> ")"
                    | <function_call>

<function_call>   ::= FN <expr>
                    | FN2 <power_no_implicit> <power_no_implicit>

<implicit_mul_power> ::= <power>
```

So in summary, we have the following features:
- Implicit multiplication: `1 2 = 1*2`, `3sin(20deg) = 3*sin(20*deg)`
- "Free" function arguments: `sin2 = sin(2)`, `log2 3 = log 2 3 = log2(3) --- (log 2 of 3)`
- Normal arithmetic grammar/infix notation.

#### List
To list the available functions, do
```bash
$ kalkcliator list
log
min
asinh
cosh
sinh
remainder
fmod
tanh
arcsin
acos
cos
atan
asin
sin
floor
tan
acosh
trunc
exp
atanh
ln
sqrt
cbrt
fabs
arctan
abs
arccos
ceil
round
max
pow
hypot
```

#### Debug
You will never have to use this argument, but for the sake of completeness, here is the usage an example output.
```bash
$ kalkcliator debug 1+2
1
+
2


--- Abstract Syntax Tree ---
BinaryNode(+)
  NumberNode(1)
  NumberNode(2)

--- Result ---
 = 3
```

## Building

### Prerequisites
- A standard C++ compiler with C++17 support
- CMake 3.10+

