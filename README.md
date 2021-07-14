# LL(1) predictive parser using C++

## Introduction
- A LL(1)[left-to-right, leftmost derivation, 1 lookahead] predictive parser which takes a LL(1) grammar and a string and tells if that string belongs to that grammar or not as well as displays the process of parsing.

## Assumptions
- Epsilon is represented by `#`.
- Production rules are in the form of `A -> alpha` where A is a non-terminal and `alpha` is combination of terminals and non-terminals
- Non-terminals should only be represented in upper case and terminals should be any character except uppercase.
- Non-terminal on the LHS of first production is assumed to be start symbol
- all the production rules associated with a non-terminal are entered in single line separated by `|` and `:`
- '$' is reserved symbol, so do not use it in production rules or string.

## Working of code

- At first left recursion is removed from the given grammar rules, if there are any using `RemoveLeftRecursion.cpp`
- Then output of the above is sent as input to `parser.cpp`.
- Then First and Follow of all the non-terminals are computed.
- with the help of First and Follow parsing table is created.
- And with the help of parse table for the grammar we check if the given string belongs to that grammar or not.
