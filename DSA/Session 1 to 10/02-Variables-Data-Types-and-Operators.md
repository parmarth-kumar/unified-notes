# Session 02 — Variables, Data Types, and Operators

**Lesson:** [Variables, Data Types & Operators](https://www.youtube.com/watch?v=Dxu7GKtdbnA) · **Prerequisite:** Session 01.

[Previous](./01-Flowcharts-Pseudocode-and-Cpp-Setup.md) · [Course map](./01-Flowcharts-Pseudocode-and-Cpp-Setup.md#course-map) · [Next](./03-Conditional-Statements-and-Loops.md)

**Core:** program structure, output, comments, variables, primitive types, conversion, input, arithmetic, comparison, logical and unary operators. **Interview extensions:** overflow, promotion before arithmetic, short-circuit guards, sequencing, floating-point limits.

**Navigate:** [Concepts](#concepts) · [Worked examples](#worked) · [Pitfalls](#pitfalls) · [Revision](#revision) · [Questions and answers](#questions).

<a id="concepts"></a>
## 1. Variables, objects, and initialisation

A variable gives a name to an object whose stored value the program can use. A **type** determines the values and operations available. `int age = 22;` declares an integer object called `age` and initialises it to 22.

Think of a labelled box as a useful beginner model. It does not imply that every local variable must occupy a separate RAM location: compilers can use registers or remove unnecessary objects while preserving observable behavior.

| Form | Meaning |
|---|---|
| `int count = 0;` | Declaration with initialisation |
| `count = 4;` | Assignment to an existing object |
| `count += 3;` | Read count, add 3, store the result |
| `const int limit = 100;` | Object cannot be reassigned through this name |
| `constexpr int sides = 4;` | Constant suitable for compile-time use here |

Names are case-sensitive. Use letters, digits, underscores; do not begin with a digit or use a keyword. Avoid reserved underscore patterns; names like `totalScore` are clear and safe. `marks` and `Marks` are different names.

**Initialise before reading.** In C++17, reading an uninitialised ordinary local `int` is generally UB, not a random-number feature. A global/static integer is zero-initialised, but relying on that distinction obscures beginner code.

## 2. Bits, bytes, sizes, and ranges

A **bit** has two possible values, 0 and 1. A C++ **byte** is the unit measured by `sizeof`; `sizeof(char) == 1` by definition. Most interview machines have 8-bit bytes; `<climits>` provides `CHAR_BIT` to inspect this. A byte is not universally required to have exactly eight bits.

| Type | Use | Typical size on common judge platforms | Important qualification |
|---|---|---:|---|
| `bool` | True/false | 1 byte | Exactly two values; storage size is implementation-specific |
| `char` | One narrow character/code unit | 1 byte | Plain `char` signedness is implementation-defined |
| `int` | Ordinary integers and indices | 4 bytes | Standard does not guarantee 32 bits |
| `long long` | Larger integer calculations | 8 bytes | At least 64 bits of required range |
| `float` | Approximate fractional values | 4 bytes | Often about 6–7 significant decimal digits |
| `double` | More precise fractional values | 8 bytes | Often about 15–16 significant decimal digits |

These sizes are common observations, not universal promises. Inspect actual limits with `numeric_limits<T>` from `<limits>`. See [fundamental types in the C++17 draft](https://timsong-cpp.github.io/cppwp/n4659/basic.fundamental) and [Microsoft's built-in type documentation](https://learn.microsoft.com/en-us/cpp/cpp/fundamental-types-cpp?view=msvc-170).

For a **32-bit two's-complement** signed integer, the usual range is `-2^31 .. 2^31 - 1`. A corresponding 32-bit unsigned integer has range `0 .. 2^32 - 1`. These formulas concern the stated width/representation. Session 06 explains why the signed range is asymmetric.

### Character and string literals

- `'A'` is a character literal; `"A"` is a string literal containing `A` followed by a terminating zero character.
- `'7'` is a character representing a digit; it is not the integer 7.
- `digit - '0'` maps `'0'..'9'` to `0..9`; decimal digit characters are contiguous in C++'s basic execution character set.
- ASCII assigns 65 to `A`, but C++ does not require every implementation to use ASCII. Label numeric character-code examples accordingly.
- `\n`, `\t`, `\\`, and `\"` represent newline, tab, backslash, and double quote in appropriate literals.

## 3. Input, output, and comments

`cin >> x` extracts a formatted value from standard input. Consecutive extractions normally skip leading whitespace; `cin >> a >> b` accepts values separated by spaces or newlines. Extraction can fail; check the stream before using the values.

`cout << x` writes a representation of `x`. `cout` normally displays bools as `1` and `0`; `boolalpha` changes that to `true` and `false`. `endl` adds a newline **and flushes** the stream; use `'\n'` when only a new line is needed.

`//` begins a line comment. `/* ... */` is a block comment; block comments do not nest. Comments should explain intent, assumptions, or the reason behind a boundary.

<!-- code: safe_sum_input -->
```cpp
#include <iostream> // cin and cout
using namespace std;

int main() {
    // Contract: both inputs are between -1,000,000,000 and 1,000,000,000.
    long long a = 0;
    long long b = 0;
    if (!(cin >> a >> b)) {
        cout << "Invalid input\n";
        return 1;
    }
    if (a < -1000000000LL || a > 1000000000LL ||
        b < -1000000000LL || b > 1000000000LL) {
        cout << "Out of range\n";
        return 1;
    }
    cout << a + b << '\n';
    return 0;
}
```

The `if` syntax is developed next session. Here it prevents calculations after invalid input and makes the arithmetic contract explicit. In a judge with guaranteed valid input, the validation may be omitted after stating those assumptions.

## 4. Arithmetic and comparison

| Family | Operators | Key idea |
|---|---|---|
| Arithmetic | `+ - * / %` | `%` works on integral operands |
| Comparison | `< <= > >= == !=` | Produces a `bool` |
| Assignment | `= += -= *= /= %=` | Updates an object |
| Logical | `&&`, `||`, `!` | Tests truth values |
| Increment/decrement | `++`, `--` | Adds/subtracts one |

### Integer division truncates toward zero

| Expression | Result | Why |
|---|---:|---|
| `7 / 2` | 3 | Both operands integral |
| `7.0 / 2` | 3.5 | Floating-point division |
| `-7 / 2` | -3 | Truncation toward zero, not floor |
| `-7 % 2` | -1 | `a = (a / b) * b + a % b` |
| `7 % -2` | 1 | Nonzero remainder has dividend's sign |

Division or remainder by zero is UB. Also, if the integer quotient cannot fit, both division and remainder are undefined; the classic example is a two's-complement minimum integer divided by `-1`. These details follow [the multiplicative-operator rules](https://timsong-cpp.github.io/cppwp/n4659/expr.mul).

Use `n % 2 != 0` to test oddness for signed numbers; `n % 2 == 1` fails for negative odd inputs.

### Logical operators and short-circuit evaluation

- `A && B` evaluates `B` only if `A` is true.
- `A || B` evaluates `B` only if `A` is false.
- `!A` reverses the truth value.
- Zero converts to false; a nonzero arithmetic value converts to true.

For built-in operators, `denominator != 0 && numerator / denominator > 2` avoids division when the denominator is zero. It still requires the quotient to be representable. Session 07 distinguishes `&&` from bitwise `&`.

### Equality is not assignment

`x == 5` compares. `x = 5` stores 5 and has a value of 5, which can convert to true inside a condition. A compiling program can therefore contain the wrong operator. Warnings and deliberate review matter.

## 5. Type conversion: inspect operands before the destination

An **implicit conversion** happens automatically. It is not necessarily a safe move from a smaller type to a larger one. Conversions can change signedness, lose precision, or discard fractional data.

A **cast** requests a conversion explicitly. Prefer a visible `static_cast<T>(value)` for simple numeric conversion; it makes intent clear but does not validate range automatically. See [Microsoft's type-safety discussion](https://learn.microsoft.com/en-us/cpp/cpp/type-conversions-and-type-safety-modern-cpp?view=msvc-170).

```text
double a = 7 / 2;                    // 3.0: integer division happened first
double b = static_cast<double>(7) / 2; // 3.5: an operand changed before division
int c = static_cast<int>(-3.9);      // -3: truncates toward zero
```

Floating-to-integer conversion is only safe if the truncated value fits the destination; out-of-range conversion is UB in C++17. See [the specification](https://timsong-cpp.github.io/cppwp/n4659/conv.fpint). A cast is not an overflow cure.

`int x{3};` uses list initialisation; `int x{3.5};` is rejected because it narrows a floating value into an integer. Small integral types such as `char` often undergo **integral promotion** before arithmetic; the expression's result need not have the same type as its inputs.

<a id="worked"></a>
## 6. Worked examples and dry runs

### Prefix versus postfix increment

<!-- code: increments -->
```cpp
#include <iostream> // cout
using namespace std;

int main() {
    int x = 5;
    int first = ++x;
    int second = x++;
    cout << first << ' ' << second << ' ' << x << '\n';
    return 0;
}
```

| Statement | Value used by expression | `x` afterwards | Destination |
|---|---:|---:|---|
| `first = ++x` | New value 6 | 6 | `first = 6` |
| `second = x++` | Previous value 6 | 7 | `second = 6` |

Output: `6 6 7`. As a separate statement, `++x;` and `x++;` have the same final effect on an ordinary integer. Do not build clever expressions that modify the same variable several times; sequencing rules are subtle.

### Promote before multiplication

Assume 32-bit `int`, and let `a = b = 100000`. Their product is `10,000,000,000`, which does not fit in `int`.

```text
long long wrong = a * b;       // multiplication is still int; UB before assignment
long long correct = 1LL * a * b; // multiply in long long from the first operation
```

<!-- code: wide_product -->
```cpp
#include <iostream> // cout
using namespace std;

int main() {
    int a = 100000;
    int b = 100000;
    long long product = 1LL * a * b;
    cout << product << '\n';
    return 0;
}
```

Dry run: `1LL * a` has type `long long` and value 100000; multiplication by `b` then occurs in that wider type. A wider type still has a finite maximum: check constraints before generalising.

### Derive a range from constraints

If at most `100000` array values each have magnitude at most `1000000000`, the total magnitude can reach `10^14`. That fits in a conventional 64-bit `long long`, but not a 32-bit `int`. This calculation determines the accumulator type before coding. Array indices and values need not use the same type.

### Precedence and associativity

`2 + 3 * 4` is 14 because multiplication groups more tightly. `(2 + 3) * 4` is 20. Assignment associates right-to-left: `a = b = 0` means `a = (b = 0)`.

**Grouping is different from evaluation order.** Precedence says how an expression is parsed; it does not generally determine which unrelated operand is evaluated first. `i++ + ++i` has unsequenced conflicting updates and is UB in C++17; do not predict a numeric answer. [C++17 sequencing rules](https://timsong-cpp.github.io/cppwp/n4659/intro.execution) define this distinction.

<a id="pitfalls"></a>
## 7. Common doubts and failure modes

| Doubt / mistake | Correct model and counterexample |
|---|---|
| “A larger destination makes intermediate arithmetic safe.” | The operands determine arithmetic type; `long long x = intA * intB` can already overflow. |
| “Every implicit conversion is safe.” | `int x = 3.9;` discards the fraction. |
| “Unsigned is simply a safer positive int.” | Unsigned arithmetic wraps modulo its range; `0U - 1U` becomes the maximum unsigned value. Mixed signed/unsigned comparisons can surprise you. |
| “Signed overflow wraps.” | C++17 gives no such guarantee; signed overflow is UB. |
| “`double` is an exact decimal type.” | Many decimal fractions, including 0.1, are approximations in common binary floating-point. |
| “`sizeof(bool)` is one bit.” | `sizeof` measures bytes; bool still has only two valid values. |
| “`0 < x < 10` tests a range.” | It parses as `(0 < x) < 10`; the first result is 0 or 1, so this is true even for `x = 100`. Use `0 < x && x < 10`. |

For approximate real-number comparisons, choose an absolute/relative tolerance appropriate to the problem. There is no magic epsilon that is correct for all scales. For integer counting and exact combinatorics, stay with integer arithmetic when possible.

<a id="revision"></a>
## 8. Rapid revision and interview teach-back

- Initialise objects; check input when input validity is not guaranteed.
- Inspect actual sizes and limits; separate typical machine behavior from C++ guarantees.
- Division uses operand types; casting the result afterwards cannot recover a lost fraction.
- Promote **before** a potentially overflowing operation.
- Logical `&&` and `||` short-circuit; guard unsafe operations on the right.
- Prefix returns the updated value; postfix returns the previous value.
- Keep side effects in separate statements.
- Arithmetic on a fixed number of fixed-width values is `O(1)` time and space; reading or printing many characters has its own cost.

**Spoken model answer:** “I choose the accumulator using the largest possible intermediate value, not just the type of one element. If the inputs are ints, I promote an operand before multiplication so that the multiplication itself uses the wider type.”

**Teach back:** explain why both `double result = 7 / 2` and `long long result = a * b` can be wrong despite their destination types.

## 9. Sources and corrections

Local note: `01_cpp_basics/02_variables_data_types_operators.md` under the notes root listed in Session 01. Code reviewed: `01-variables-and-data-types/q3-simple-interest.cpp` and `02-operators/constants.cpp` under the code root. These prompted the explicit discussion of numeric type choice, constants, and arithmetic bounds. Prefer typed constants over beginner macros such as `#define X 25`.

Primary references are linked beside the relevant rules above. Examples assume a normal hosted C++17 implementation; platform-specific output such as exact `sizeof(int)` is deliberately not asserted universally.

<a id="questions"></a>
## 10. Revision questions — theory, trace, debug, implement

1. Distinguish declaration, initialisation, and assignment. What is wrong with reading an ordinary uninitialised local `int`? [Review](#concepts)
2. Which of `sizeof(char) == 1`, `sizeof(int) == 4`, and `CHAR_BIT == 8` are guaranteed universally? [Review](#concepts)
3. Distinguish `'5'`, `5`, and `"5"`. How do you convert a validated digit character to a number? [Review](#concepts)
4. Predict `7 / 2`, `-7 / 2`, `-7 % 2`, and `7 % -2`. [Review](#concepts)
5. Why is `double answer = 9 / 4` equal to 2.0? Give two fixes. [Review](#concepts)
6. Does `static_cast<int>(-9.8)` round? Are all floating-to-int casts defined? [Review](#concepts)
7. Trace `int x = 4; int a = x++; int b = ++x;`. Give all final values. [Review](#worked)
8. Why is `long long product = a * b` unsafe for large `int` operands? Fix it. [Review](#worked)
9. Explain the short-circuit behavior of `&&` and `||`. Construct a division guard. [Review](#concepts)
10. Repair `0 <= score <= 100`. Give an input exposing the bug. [Review](#pitfalls)
11. What is the difference between precedence and evaluation order? Should you predict `i++ + ++i`? [Review](#worked)
12. A sum has `2 * 10^5` terms of magnitude at most `10^9`. Which conventional integer type is suitable, and why? [Review](#worked)
13. **Implementation:** write a program reading two bounded integers, rejecting malformed input, and printing their sum. [Review](#concepts)
14. **Debug:** a program claims `n % 2 == 1` detects every odd signed integer. Give a counterexample and fix it. [Review](#concepts)
15. **Interview:** explain signed overflow, unsigned wraparound, and why a cast does not guarantee safety. [Review](#pitfalls)

<details>
<summary>Answer key</summary>

1. Declare a name/type; provide its initial value; later replace its value. Reading the uninitialised local integer is generally UB in C++17.
2. Only `sizeof(char) == 1`. The other two are common, not universal.
3. Character, integer, string literal. After validating `'0' <= c && c <= '9'`, use `c - '0'`.
4. `3, -3, -1, 1`.
5. Integral division finishes first. Use `9.0 / 4` or `static_cast<double>(9) / 4`.
6. It truncates to -9. Out-of-range truncated results have UB.
7. `a = 4`, `b = 6`, `x = 6`.
8. The product is evaluated as int first. Use `1LL * a * b`, assuming the wider product fits.
9. AND skips the right side when left is false; OR skips when left is true. Test denominator nonzero before division, with representable quotient.
10. `0 <= score && score <= 100`; `score = 1000` exposes the original chain.
11. Parsing/grouping versus temporal evaluation. The conflicting unsequenced updates are UB; no guaranteed numeric result.
12. Bound is `2 * 10^14`; conventional 64-bit `long long` fits it.
13. Section 3 gives a complete version and its contract. Test valid negatives, limits, and a nonnumeric token.
14. `-3 % 2` is -1; use `n % 2 != 0`.
15. Signed arithmetic outside range is UB; unsigned arithmetic is modular; conversions and prior intermediate arithmetic still need range checks.

</details>
