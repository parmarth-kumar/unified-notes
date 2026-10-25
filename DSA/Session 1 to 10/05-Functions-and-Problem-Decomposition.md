# Session 05 — Functions and Problem Decomposition

**Lesson:** [Functions](https://www.youtube.com/watch?v=P08Z_NC8GuY) · **Prerequisites:** expressions, conditionals, loops.

[Previous](./04-Patterns-and-Nested-Loops.md) · [Course map](./01-Flowcharts-Pseudocode-and-Cpp-Setup.md#course-map) · [Next](./06-Binary-Number-System-and-Conversions.md)

**Core:** definitions, calls, parameters, return values, pass by value, sum/product, factorial, digit sum, nCr. **Extensions:** references, const references, scope/lifetime, contracts, safe combinations, decomposition for interviews.

**Navigate:** [Mechanics](#mechanics) · [Parameter passing](#parameters) · [Worked functions](#worked) · [Combinations](#combinations) · [Revision](#revision) · [Questions](#questions).

<a id="mechanics"></a>
## 1. A function packages one meaningful operation

A function is a named operation with an interface: inputs, a result or side effect, and a contract. For example, `isPrime(n)` asks a meaningful question while hiding the divisor loop. This makes a larger algorithm easier to explain and reuse.

- **Declaration:** tells the compiler a function's name and type.
- **Definition:** supplies its body.
- **Call:** requests execution with arguments.
- **Parameter:** a named input in the function definition.
- **Argument:** the value/expression supplied at a call.
- **Return type:** type of the result; `void` means no returned value.

<!-- code: function_call -->
```cpp
#include <iostream> // cout
using namespace std;

int add(int a, int b); // declaration; contract: the sum fits in int

int main() {
    int x = 7;
    int y = 4;
    int answer = add(x, y);
    cout << answer << '\n';
    return 0;
}

int add(int a, int b) { // definition
    return a + b;
}
```

The declaration makes `add` known before `main` calls it. Another arrangement is to place the full definition before `main`. C++ does not generally let you call an undeclared function and discover its interface later.

### Dry run of the call

1. `main` has x = 7 and y = 4.
2. The call initialises the value parameters a = 7 and b = 4.
3. `return a + b` supplies 11 to the caller and ends this invocation.
4. `main` stores 11 in answer and prints it.

Returning a value is different from printing it. A function that only prints 11 does not supply 11 for use in a later expression. Prefer returning computed results and letting the caller decide how to display them.

### Return paths

`return` stops the current function, including any loops inside it. Every reachable normal path in a non-void function should produce the promised result. Flowing off the end of an ordinary non-void value-returning function is UB in C++17; `main` is a special case with an implicit return of zero.

Do not confuse `void` with “does nothing”: a void function can modify data or print output. Its effect simply is not a returned value.

## 2. Calls, stack frames, and the limits of the model

The **call stack** is the usual implementation model for nested calls: the newest active call returns first, which is last-in, first-out. A **stack frame** commonly holds saved execution state and some parameters/local storage.

```text
main calls solve
solve calls helper
helper returns -> solve resumes
solve returns  -> main resumes
```

This is a mental model, not a guarantee that each C++ call allocates a physical frame or every local lives in RAM. Optimisation can inline a function, keep values in registers, or eliminate unused work. Recursion is a later topic; an unbounded sequence of active recursive calls can exhaust implementation resources.

## 3. Scope and lifetime

**Scope** is where a name can be used in source code. **Lifetime** is when an object exists during execution. They answer different questions.

A block-local variable is ordinarily usable from its declaration to the end of its block, and its ordinary automatic object is destroyed when execution leaves the block. A parameter is local to its function invocation. Two functions can each have a local named `count` without sharing an object.

Never return a reference or pointer to an ordinary local object that dies on return. The caller would receive a **dangling** reference/pointer, which no longer refers to a live object. Returning an integer value is safe; the result is supplied by value rather than exposing a dead local.

<a id="parameters"></a>
## 4. Value, reference, and const reference

| Parameter | Meaning | Can modify caller's object through it? | Typical use |
|---|---|---|---|
| `int x` | Independent integer value | No | Small scalar input |
| `int& x` | Alias to caller's integer | Yes | Explicit in-place update |
| `const int& x` | Read-only alias | No | Usually unnecessary for a tiny int |
| `const vector<int>& a` | Read-only vector alias | No | Inspect a vector without copying its elements |
| `vector<int>& a` | Mutable vector alias | Yes | Modify a caller's vector |

The vector forms are previews for Session 09. A **reference** acts as another name for an existing object. `const` restricts modification through that access path; it is not a claim that no other alias can change the object.

<!-- code: value_reference -->
```cpp
#include <iostream> // cout
using namespace std;

int changeCopy(int value) {
    value += 100;
    return value;
}

void changeOriginal(int& value) {
    value += 100;
}

int main() {
    int score = 5;
    changeCopy(score); // the returned copy is deliberately ignored
    cout << score << ' ';
    changeOriginal(score);
    cout << score << '\n';
    return 0;
}
```

Output: `5 105`. In the first call, modifying the local copy does not modify score, and the caller ignores the returned value. Assigning that return value back to score would be a separate caller action. In the second call, the parameter aliases score. Passing a reference is useful when mutation is part of the interface; a return value is often clearer for a simple calculation.

**Important preview:** `void f(int a[])` is adjusted to a pointer parameter; it is not a C++ reference parameter. A pointer passed by value can still refer to caller-owned elements. Session 08 develops this distinction.

<a id="worked"></a>
## 5. Factorial: identity, contract, and overflow

For n >= 1, `n! = 1 * 2 * ... * n`. Define `0! = 1`, consistent with the empty product and the recurrence `n! = n * (n-1)!`.

On a conventional signed 64-bit integer, 20! fits but 21! does not. A 32-bit signed int holds 12!, but not 13!. These are representation-specific capacity facts, not permission to use unbounded factorial loops. Factorial growth is faster than any fixed-base exponential as n grows.

<!-- code: factorial -->
```cpp
#include <stdexcept> // invalid_argument
using namespace std;

long long factorial(int n) {
    if (n < 0 || n > 20) {
        throw invalid_argument("factorial requires 0 <= n <= 20");
    }
    long long result = 1;
    for (int factor = 2; factor <= n; ++factor) {
        result *= factor;
    }
    return result;
}
```

For n = 5, the successive results are `1 -> 2 -> 6 -> 24 -> 120`. Before multiplying by factor, result contains `(factor - 1)!`. At exit, it contains n!. When n is 0 or 1, no iteration is needed and 1 is correct.

Time is `O(n)` multiplications and extra space `O(1)` under fixed-width arithmetic. With the literal n <= 20 contract, actual work is bounded by a constant; the `O(n)` description explains the general loop structure.

## 6. Sum of decimal digits, safely including negative int values

For 507, the desired sum is `5 + 0 + 7 = 12`. Repeated `% 10` extracts a last digit, and `/= 10` removes it.

Calling `abs(n)` on a two's-complement minimum int is a trap: its positive magnitude cannot fit in int. Instead, handle one remainder at a time. The remainder is always between -9 and 9, so negating a negative digit is safe.

<!-- code: digit_sum -->
```cpp
using namespace std;

int digitSum(int n) {
    int sum = 0;
    while (n != 0) {
        int digit = n % 10;
        sum += (digit < 0) ? -digit : digit;
        n /= 10;
    }
    return sum;
}
```

### Dry run for -507

| n before extraction | `n % 10` | Added magnitude | sum | n after division |
|---:|---:|---:|---:|---:|
| -507 | -7 | 7 | 7 | -50 |
| -50 | 0 | 0 | 7 | -5 |
| -5 | -5 | 5 | 12 | 0 |

For zero, return 0: this is the digit **sum**, not the digit count. For d decimal digits, time is `Theta(d)` and auxiliary space `O(1)`. For nonzero n, d grows like `log10(|n|)`; write `O(log(|n| + 1))` with a constant-time zero case rather than taking `log(0)`.

## 7. Reusing helpers without hiding cost

To print primes from 2 through n, use the `isPrime` helper from Session 03. This improves readability, but it does not make the mathematical work disappear. Calling an `O(sqrt(value))` helper for every candidate gives a simple `O(n sqrt(n))` upper bound overall.

Likewise, a function containing an n-element loop still takes linear time even though its call is one line. Analyse the implementation and the number of calls.

<a id="combinations"></a>
## 8. nCr: choosing r items without regard to order

`C(n, r)`, read “n choose r”, counts ways to choose r distinct items from n distinct items when order does not matter. The factorial formula is:

```text
C(n, r) = n! / (r! * (n-r)!)
```

For n = 5 and r = 2, the result is 10. Choosing A then B is the same selection as choosing B then A; permutations would count order separately. `C(n,0) = C(n,n) = 1` and `C(n,r) = C(n,n-r)`.

### First implementation: bounded factorials

<!-- code: choose_factorial -->
```cpp
#include <stdexcept> // invalid_argument
using namespace std;

long long chooseByFactorial(int n, int r) {
    if (n < 0 || n > 20 || r < 0 || r > n) {
        throw invalid_argument("require 0 <= r <= n <= 20");
    }
    auto fact = [](int value) {
        long long result = 1;
        for (int factor = 2; factor <= value; ++factor) {
            result *= factor;
        }
        return result;
    };
    return fact(n) / (fact(r) * fact(n - r));
}
```

`auto fact = ...` defines a small local helper called a **lambda**; it lets this example compile independently. You can instead use the named factorial helper from Section 5. Under the stated domain, numerator and denominator fit signed 64-bit arithmetic. Since `C(n,r) >= 1`, the denominator product is no greater than n!.

Time is `O(n)` total loop work, not `O(n³)`: these factorial calls happen sequentially. The problem is range, not asymptotic time alone. Even if a final combination fits, n! might not.

### Interview extension: cancel before multiplying

A recurrence computes combinations without whole factorials:

```text
result initially 1
for i = 1..r
    result = result * (n-r+i) / i
```

Use `r = min(r, n-r)` to reduce work. But multiplication can overflow **before** the exact division. To make a wider bounded implementation safe, cancel common factors first using the greatest common divisor (**gcd**).

<!-- code: choose_cancelled -->
```cpp
#include <algorithm> // min
#include <numeric> // gcd, available in C++17
#include <stdexcept> // invalid_argument
using namespace std;

// All C(n, r) for 0 <= r <= n <= 66 fit signed 64-bit long long.
long long choose(int n, int r) {
    if (n < 0 || n > 66 || r < 0 || r > n) {
        throw invalid_argument("require 0 <= r <= n <= 66");
    }
    r = min(r, n - r);
    long long result = 1;
    for (int i = 1; i <= r; ++i) {
        long long numerator = n - r + i;
        long long denominator = i;
        long long common = gcd(numerator, denominator);
        numerator /= common;
        denominator /= common;
        common = gcd(result, denominator);
        result /= common;
        denominator /= common;
        // The remaining denominator is 1 because the next combination is integral.
        result *= numerator;
    }
    return result;
}
```

For `C(5,2)`: i = 1 gives `1 * 4 / 1 = 4`. At i = 2, numerator 5 and denominator 2 have gcd 1; cancel denominator 2 against result 4 to get result 2, then multiply by 5 to get 10. No intermediate product of 20 is needed.

**Why cancellation completes:** after removing the numerator's common factors with the denominator, the remaining denominator is coprime to the numerator. Since `result * numerator / denominator` is an integer combination, the remaining denominator must divide result. Each new result equals `C(n-r+i, i)` and is no greater than the final result along this sequence. The bound n <= 66 keeps every final/intermediate combination within signed 64-bit capacity.

For r iterations with gcd computations, the usual numeric-value bound is `O(r log n)` arithmetic work; treating machine-width gcd as bounded gives `O(r)` word-level steps. Space is constant. You do not need to memorise this extension before mastering the basic functions; understand the multiply-before-divide hazard first.

## 9. Interview interface choices and common doubts

- **Overloading:** functions can share a name with different parameter lists; return type alone cannot distinguish overloads.
- **Default arguments:** a declaration can supply a default input, but the function still has its declared parameter list. Avoid adding defaults before you understand the interface.
- **Inline:** the `inline` keyword is not a guarantee of inlining or speed. Correctness should not depend on compiler optimisation choices.
- **Pure calculation:** a function that computes a result without external mutation is easier to reuse and test. Printing or mutation can still be appropriate when part of the contract.
- **Error handling:** exceptions, optional results, or explicit status outputs are alternatives. Do not use an ambiguous sentinel such as -1 if -1 can be a valid result.

For an interview, say: “This helper takes read-only input and returns the computed value. If mutation is intended, I will make that explicit in the reference parameter. I will also state the domain so invalid input does not silently produce a plausible result.”

<a id="revision"></a>
## 10. Rapid revision and teach-back

- A declaration exposes an interface; a definition supplies behavior; a call supplies arguments.
- A value parameter is independent; a reference aliases an existing object.
- Return supplies a result; output displays it.
- Scope concerns names; lifetime concerns live objects.
- Do not return a reference/pointer to a dead local.
- `0! = 1`; factorial range must be bounded.
- Digit extraction on signed input can avoid `abs(INT_MIN)`.
- A small final answer does not prove that its intermediate arithmetic fits.
- Count helper calls and helper cost together.

**Teach back:** demonstrate both parameter-passing forms, explain the zero-factorial base case, trace negative digit sum, and give a counterexample to unbounded factorial-based nCr.

## 11. Sources

Local note: `01_cpp_basics/05_functions.md`. Code reviewed: `06-functions/08_practice_q_sum_digit.cpp` and `06-functions/05_binomial_coeff.cpp`. This session replaces unbounded factorial arithmetic and unsafe whole-number absolute value with explicit contracts and safe alternatives.

Language reference: [C++17 function declarations](https://timsong-cpp.github.io/cppwp/n4659/dcl.fct) and [execution model](https://timsong-cpp.github.io/cppwp/n4659/intro.execution). Stack-frame descriptions above are intentionally labelled as an implementation model.

<a id="questions"></a>
## 12. Revision questions

1. Define declaration, definition, call, parameter, argument, and return type. [Review](#mechanics)
2. Why might a valid definition below main still need a declaration above main? [Review](#mechanics)
3. Explain returning 10 versus printing 10. Can a void function modify caller-owned data? [Review](#mechanics)
4. Trace the value/reference program and explain why its outputs differ. [Review](#parameters)
5. Does every function call necessarily create a physical stack frame? [Review](#mechanics)
6. Distinguish scope and lifetime. Why is returning a reference to an ordinary local invalid? [Review](#mechanics)
7. **Implementation:** write factorial with a safe stated domain. Explain 0!, invariant, time, and space. [Review](#worked)
8. **Trace:** compute the digit sum of -9020 using signed remainders. What is the result for zero? [Review](#worked)
9. Why can `abs(INT_MIN)` be unsafe on a two's-complement machine? How does the digit-sum code avoid it? [Review](#worked)
10. Derive `C(5,2)` and explain why order is not counted. [Review](#combinations)
11. Why is factorial-based nCr unsafe for arbitrary n even when the final combination is small? [Review](#combinations)
12. What is the hazard in `result * numerator / denominator`? Explain cancellation on `C(5,2)`. [Review](#combinations)
13. **Debug:** a non-void function returns only inside one branch. What must you inspect? [Review](#mechanics)
14. **Interview:** a function calls a linear helper n times. Is the whole function linear just because each call is one statement? [Review](#worked)
15. Can you overload two functions differing only in return type? Does inline guarantee a faster call? [Review](#revision)

<details>
<summary>Answer key</summary>

1. Interface announcement; body; execution request; named formal input; actual supplied expression; type of result.
2. The call must see a declaration to check its interface.
3. Returning supplies a value to an expression; printing performs an output effect. Yes, for example via a mutable reference.
4. 5 then 105; first modifies a copy, second aliases score.
5. No; optimisation can inline or eliminate a call and keep storage in registers.
6. Name visibility versus object existence. The ordinary local dies on return; the exposed reference would dangle.
7. Section 5, n in [0,20]. Empty product 1; result before factor is `(factor-1)!`; general loop O(n) time, O(1) space.
8. Remainders 0,-2,0,-9 contribute 0,2,0,9, total 11. Zero's digit sum is 0.
9. The positive magnitude does not fit int. Only digit remainders -9..9 are negated.
10. 10; selecting a pair ignores ordering, so AB and BA are one choice.
11. The numerator factorial can overflow long before the final quotient; for example C(30,1) = 30 but 30! exceeds 64-bit range.
12. Multiplication may overflow first. In step 2, cancel denominator 2 against the existing result 4, then multiply 2 by 5.
13. All reachable normal paths need a result; falling through an ordinary value-returning function is UB.
14. No. If each call costs O(n) and there are n calls, O(n²) work may result. Analyse actual sizes if they vary.
15. No; return type alone is insufficient. Inline does not guarantee physical inlining or speed.

</details>
