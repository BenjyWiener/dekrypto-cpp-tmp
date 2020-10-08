# dekrypto (C++ TMP)
An extremely crude Krypto puzzle brute-force solver written as a C++ template metaprogram

## What is Krypto?
> The Krypto deck consists of 56 cards: three of each of the numbers 1-6, four each of the numbers 7-10, two each of 11-17, one each of 18-25. Six cards are dealt: a common objective card at the top and five other cards below. Each player must use all five of the cards' numbers exactly once, using any combination of arithmetic operations (addition, subtraction, multiplication, and division), to form the objective card's number. The first player to come up with a correct formula is the winner.
> – [Wikipedia](https://en.wikipedia.org/wiki/Krypto_(game))

## What is C++ Template Metaprogramming (TMP)?
> Template meta-programming (TMP) refers to uses of the C++ template system to perform computation at compile-time within the code. It can, for the most part,
> be considered to be "programming with types" — in that, largely, the "values" that TMP works with are specific C++ types. Using types as the basic objects of
> calculation allows the full power of the type-inference rules to be used for general-purpose computing.
> – [Wikibooks | C++ Programming/Templates/Template Meta-Programming](https://en.wikibooks.org/wiki/C%2B%2B_Programming/Templates/Template_Meta-Programming)

## Why?
Cuz.

#### `template <const char *BadThingIDid>`
## Why did you `BadThingIDid`?

🤷‍♂️. This was my hands-on/learn-it-by-doing-it project for C++ TMP. I experimented with a bunch of different things, and couldn't bother to tidy up after.

## Whatever. How does it work?
Input is provided via two text files in the subdirectory `input/` that are `#include`d at compile time (which is actually runtime in this case).
- `cards.txt`: A comma separated list of integers representing list of integers representing the drawn cards (excluding the goal/objective card)
- `goal.txt`: A single integer representing the goal/objective card

Compile the program with your favorite C++20 compliant compiler, and run the program to print out the solutions.
Note: Although the program technically supports any non-zero number of cards, anything more than 3 will take a while and probably require additional compiler flags
to increase the maximum template instantiation depth.
